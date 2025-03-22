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

const uint8_t PrimaryID = 254;                      //Used to request adoption
const uint16_t TotalLEDs = 1;                       //The total amounts of LEDs in the strip
CRGB LEDs[TotalLEDs];                               //Array with our status LED
CRGB ColorBoot = CRGB(255, 128, 0);                 //While starting up
CRGB ColorGetID = CRGB(255, 0, 0);                  //While trying to get an IP
CRGB ColorWaitForAdoptionIdle = CRGB(255, 255, 0);  //While waiting for Primary to adopt us
CRGB ColorIdle = CRGB(0, 0, 255);                   //While idle/normal state
CRGB ColorDispencing = CRGB(0, 255, 0);             //While dispensing
uint8_t FluidID = 0;                                //SAVESETTING Store the fluid of this dispenser
uint8_t MSperML = 40;                               //SAVESETTING ms to let 1 ml go, for example it takes 12s to do 300ml, thats about 40 ms per ml
uint8_t DelayAir = 0;                               //SAVESETTING ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
unsigned long lastPulseTime = 0;                    //Time of last revieved pulse from SLOT
bool ImAdopted = false;                             //If the primary has seen this dispenser yet
bool LEDmode = 0;                                   //use to enable rainbow led mode
enum COMMANDS { UNK,
                ADOPT,
                DISPENSE,
                CALIBRATEMSPERML,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR,
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

  LEDloop();  //Do the LED update loop if needed, this is for rainbow.
}
void LEDloop() {
  EVERY_N_MILLISECONDS(40) {
    switch (LEDmode) {
      case 1:
        static uint8_t gHue;
        gHue++;
        fill_rainbow(&LEDs[0], 1, gHue, 1);
        FastLED.show();
        break;
    }
  }
}
void LoadSettings() {
  EEPROM.begin(16);  //Needed for ESP32
  FluidID = EEPROM.read(0);
  DelayAir = EEPROM.read(1);
  MSperML = EEPROM.read(2);
  for (uint8_t i = 0; i < 3; i++) {
    Serial.println("LoadedSetting " + String(i) + "=" + EEPROM.read(i));
  }
}
void SaveSettings() {
  EEPROM.write(0, FluidID);
  EEPROM.write(1, DelayAir);
  EEPROM.write(2, MSperML);
  EEPROM.commit();  //Needed for ESP32
  for (uint8_t i = 0; i < 3; i++) {
    Serial.println("SavedSetting " + String(i) + "=" + EEPROM.read(i));
  }
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
    uint8_t BusSend[] = { DISPENSERSTATUS, result };          //Reply back we have completed
    uint16_t result2 = bus.reply(&BusSend, sizeof(BusSend));  //Send success Primary
    if (result2 == PJON_FAIL) Serial.print("bus.reply wrong =" + String(result2));
  }
}
uint8_t triggerAction(uint8_t cmd1, uint8_t cmd2) {
  /*
    1=DISPENSE, Time_in_ms*100
    2=GETFLUID, IGNORED
    3=CHANGEFLUID, New_ID
    4=CHANGEDELAY, Time_in_ms*5 between air and fluid   
    5=CHANGECOLOR, RRGGBBMM
  */
  uint8_t r, g, b, m;
  Serial.println(FluidID);
  switch (cmd1) {
    case ADOPT:
      Serial.println("ADOPT");
      ImAdopted = true;
      SetLEDidleColor();
      return FluidID;
      break;
    case DISPENSE:
      Serial.println("DISPENSE for " + String(cmd2) + "*" + String(MSperML) + "=" + String(cmd2 * MSperML) + "ms");
      DispenseStart();
      delay(cmd2 * MSperML);
      DispenseStop();
      break;
    case CALIBRATEMSPERML:
      Serial.println("CALIBRATEMSPERML");
      MSperML = cmd2;
      SaveSettings();
      break;
    case CHANGEFLUID:
      Serial.println("CHANGEFLUID");
      FluidID = cmd2;
      SaveSettings();
      break;
    case CHANGEDELAY:
      Serial.println("CHANGEDELAY");
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
      m = cmd2 & 0x03;           //Mask with 0x03 (00000011)
      if (m == 0b00) {
        SetLEDidleColor();
      }
      if (m == 0b01) {
        fill_solid(&(LEDs[0]), TotalLEDs, CRGB(r, g, b));
        FastLED.show();
      }
      if (m == 0b10) {
        if (LEDs[0] == ColorIdle)  //Do not overwrite others, only listen in normal idle state
          LEDmode = 1;
      }
      Serial.println("r=" + String(r) + " g=" + String(g) + " b=" + String(b) + " m=" + String(m));
      //TODO, we can define color modes
      //m 00 = Auto
      //m 01 = RGB mode,
      //m 10 = rainbow mode
      //m 11 = reserved for other modesDelayAir
      break;
    default:
      return false;
      break;
  }
  return true;
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
  LEDmode = 0;
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
    uint8_t ID1 = GetSlotID();
    delay(10);
    uint8_t ID2 = GetSlotID();
    delay(10);
    uint8_t ID3 = GetSlotID();
    if (ID1 > 0 && ID1 == ID2 && ID2 == ID3) {
      bus.set_id(ID1);
      if (LEDs[0] != ColorDispencing)  //Do not overwrite ColorDispencing
        SetLEDidleColor();
      Serial.println("SlotID recieved, I am " + String(bus.device_id()));

      uint8_t BusSend[] = { ADOPT, bus.device_id() };  //Ask Primary for us to be adopted
      uint16_t result = bus.send(PrimaryID, &BusSend, sizeof(BusSend));
      if (result == PJON_FAIL) Serial.print("bus request wrong =" + String(result));

    } else {
      Serial.println("Error in SlotID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
    }
  }
}
uint8_t GetSlotID() {
  Serial.println(String(millis()) + " GetSlotID");
  pinMode(PDI_SLOT_TXRX, OUTPUT);
  digitalWrite(PDI_SLOT_TXRX, LOW);
  delay(1);
  pinMode(PDI_SLOT_TXRX, INPUT);
  uint8_t pulseCount = 0;  //Amount of pulses counted from SLOT
  unsigned long lastPulseTime = millis();
  bool OLD_PIN_state = 0;
  while (millis() - lastPulseTime < 10) {  //Do NOT put Serial in this while loop. It will be to slow to count the pulses
    bool PIN_state = digitalRead(PDI_SLOT_TXRX);
    if (PIN_state != OLD_PIN_state) {  //Only update if state changes
      OLD_PIN_state = PIN_state;
      if (PIN_state == LOW)  //Only count if the line goes low
        pulseCount++;
      lastPulseTime = millis();  //Reset timeout on pulse detection
    }
  }
  return pulseCount;
}