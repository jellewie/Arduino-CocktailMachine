/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
  DISPENSER
*/
#include <FastLED.h>  //Include the libary FastLED (If you get a error here, make sure it's installed!)
#include <EEPROM.h>
#include <PJONSoftwareBitBang.h>
#define SWBB_MODE 1  //Should be default, 1=1.97kB/s //https://github.com/gioblu/PJON/blob/master/src/strategies/SoftwareBitBang/README.md#performance
//#define SWBB_MAX_ATTEMPTS 20;	//Maximum transmission attempts	Numeric value (20 by default)

const uint8_t PDI_Button = 14;      //Pull down to trigger
const uint8_t PDO_ValveFluid = 33;  //LOW=OFF
const uint8_t PDO_ValveAir = 32;    //LOW=OFF
const uint8_t PDI_SLOT_TXRX = 13;   //The wire from which to get local ID from    //must be 2 or 3 on attiny85 I think
const uint8_t PAO_LED = 26;         //To which pin the <LED> is connected to
const uint8_t PDIO_buspin = 25;

const uint16_t TotalLEDs = 1;  //The total amounts of LEDs in the strip
CRGB LEDs[TotalLEDs];
CRGB ColorBoot = CRGB(255, 128, 0);
CRGB ColorGetID = CRGB(255, 0, 0);
CRGB ColorWaitForAdoptionIdle = CRGB(255, 255, 0);
CRGB ColorIdle = CRGB(0, 0, 255);
CRGB ColorDispencing = CRGB(0, 255, 0);
uint8_t FluidID = 0;                                //Store the fluid of this dispenser
unsigned long lastPulseTime = 0;                    //Time of last revieved pulse from SLOT
uint16_t DelayAir = 0;                              //ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
bool ImAdopted = false;
enum COMMANDS { UNK,
                DISPENSE,
                GETFLUID,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR,
                ADOPT };
enum COMMAND { DISPENSERUNK,
               DISPENSERSTATUS,
};
PJONSoftwareBitBang bus;  //DeviceID = PJON_NOT_ASSIGNED
void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);
  pinMode(PDO_ValveFluid, OUTPUT);
  pinMode(PDO_ValveAir, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PDO_ValveFluid, LOW);  //Ensure valve is OFF at start
  digitalWrite(PDO_ValveAir, LOW);    //Ensure valveAir is OFF at start
  LoadSettings();
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);
  fill_solid(&(LEDs[0]), TotalLEDs, ColorBoot);  //RED to show we do not have an ID
  FastLED.show();
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
  Serial.println("Start done");
}
void loop() {
  CheckAndGetSlotID();  //Try and get the SLOT ID

  //Manual control
  bool button_state = digitalRead(PDI_Button);
  static bool OLD_button_state = button_state;
  if (button_state != OLD_button_state) {  //Only update if state changes
    OLD_button_state = button_state;
    (button_state == LOW) ? DispenseStart() : DispenseStop();  //Button pressed
  }

  bus.receive(50000);  // Receive messages
  bus.update();        // Handle bus updates
void LoadSettings() {
  FluidID = EEPROM.read(0);
  DelayAir = EEPROM.read(1);
}
void SaveSettings() {
  EEPROM.write(0, FluidID);
  EEPROM.write(1, DelayAir);
}
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if (code == PJON_CONNECTION_LOST) {
    Serial.print("Connection with device ID ");
    Serial.print(bus.packets[data].content[0], DEC);
    Serial.println(" is lost.");
  }
  if (code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data, DEC);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
  }
  if (code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
  }
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println();
  if (length == 2) {
    uint8_t result = triggerAction(payload[0], payload[1]);
    Serial.println("called triggerAction with result=" + String(result));
    uint8_t BusSend[] = { DISPENSERSTATUS, true };            //Reply back we have completed succesfully
    uint16_t result2 = bus.reply(&BusSend, sizeof(BusSend));  //Send success Primary
    if (result2 == PJON_FAIL) Serial.print("bus.reply wrong =" + String(result2));
  }
}
byte triggerAction(byte cmd1, byte cmd2) {
  /*
    1=DISPENSE, Time_in_ms*10
    2=GETFLUID, IGNORED
    3=CHANGEFLUID, New_ID
    4=CHANGEDELAY, Time_in_ms*5 between air and fluid   
    5=CHANGECOLOR, RRGGBBMM
  */
  byte r, g, b, m;
  Serial.println("Action=" + String(cmd1) + " triggered with variable=" + String(cmd2));
  switch (cmd1) {
    case DISPENSE:
      DispenseStart();
      delay(cmd2 * 10);
      DispenseStop();
      break;
    case GETFLUID:
      return FluidID;
      break;
    case CHANGEFLUID:
      FluidID = cmd2;
      SaveSettings();
      break;
    case CHANGEDELAY:
      DelayAir = cmd2 * 5;
      SaveSettings();
      break;
    case CHANGECOLOR:
      Serial.println("CHANGECOLOR");
      r = (cmd2 >> 6) & 0b11;    // Extract bits 7-6 (RR)
      g = (cmd2 >> 4) & 0b11;    // Extract bits 5-4 (GG)
      b = (cmd2 >> 2) & 0b11;    // Extract bits 3-2 (BB)
      r = map(r, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      g = map(g, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      b = map(b, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      Serial.println("r=" + String(r) + " g=" + String(g) + " b=" + String(b));
      fill_solid(&(LEDs[0]), TotalLEDs, CRGB(r, g, b));
      FastLED.show();
      m = cmd2 & 0x03;  //Mask with 0x03 (00000011)
      if (m == 0b01) {
      }
      //TODO, we can define color modes
      //m 00 = Auto
      //m 01 = RGB mode,
      //m 10 = rainbow mode
      //m 11 = reserved for other modesDelayAir
      break;
    case ADOPT:
      ImAdopted = true;
      SetLEDidleColor();
      break;
    default:
      return 4;
      break;
  }
  return 200;
}
void DispenseStart() {
  Serial.println("DispenseStart");
  fill_solid(&(LEDs[0]), TotalLEDs, ColorDispencing);
  FastLED.show();
  digitalWrite(PDO_ValveAir, HIGH);
  delay(DelayAir);
  digitalWrite(PDO_ValveFluid, HIGH);
}
void DispenseStop() {
  Serial.println("DispenseStop");
  digitalWrite(PDO_ValveAir, LOW);
  digitalWrite(PDO_ValveFluid, LOW);
  SetLEDidleColor();
}
void SetLEDidleColor() {
  if (ImAdopted) {
    fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
  } else {
    fill_solid(&(LEDs[0]), TotalLEDs, ColorWaitForAdoptionIdle);
  }
  FastLED.show();
}
void CheckAndGetSlotID() {
  if (bus.device_id() == PJON_NOT_ASSIGNED) {
    Serial.println("SlotID unknown (" + String(bus.device_id()) + ")");
    if (LEDs[0] != ColorGetID and LEDs[0] != ColorDispencing) {  //If not yet desired color, but do not overwrite ColorDispencing
      fill_solid(&(LEDs[0]), TotalLEDs, ColorGetID);
      FastLED.show();
    }
    byte ID1 = GetSlotID();
    delay(10);
    byte ID2 = GetSlotID();
    delay(10);
    byte ID3 = GetSlotID();
    if (ID1 > 0 && ID1 == ID2 && ID2 == ID3) {
      bus.set_id(ID1);
      if (LEDs[0] != ColorDispencing)  //Do not overwrite ColorDispencing
        SetLEDidleColor();
      Serial.println("SlotID recieved, I am " + String(bus.device_id()));
    } else {
      Serial.println("Error in SlotID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
    }
  }
}
byte GetSlotID() {
  Serial.println(String(millis()) + " GetSlotID");
  pinMode(PDI_SLOT_TXRX, OUTPUT);
  digitalWrite(PDI_SLOT_TXRX, LOW);
  delay(1);
  pinMode(PDI_SLOT_TXRX, INPUT);
  int pulseCount = 0;  //Amount of pulses counted from SLOT
  unsigned long lastPulseTime = millis();
  bool OLD_PIN_state = 0;
  while (millis() - lastPulseTime < 20) {  //Do NOT put Serial in this while loop. It will be to slow to count the pulses
    bool PIN_state = digitalRead(PDI_SLOT_TXRX);
    if (PIN_state != OLD_PIN_state) {  //Only update if state changes
      OLD_PIN_state = PIN_state;
      if (PIN_state == LOW)
        pulseCount++;
      lastPulseTime = millis();  //Reset timeout on pulse detection
    }
  }
  return pulseCount;
}