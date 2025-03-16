/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
  DISPENSER
*/
#include <FastLED.h>  //Include the libary FastLED (If you get a error here, make sure it's installed!)
#include <EEPROM.h>
#include <PJONSoftwareBitBang.h>

const byte DeviceID = 1;  //Unique 1-byte ID (change for every dispenser). must be higher than 0

const byte PDI_Button = 14;      //Pull down to trigger
const byte PDO_ValveFluid = 33;  //LOW=OFF
const byte PDO_ValveAir = 32;    //LOW=OFF
const byte PDI_SLOT_TXRX = 13;   //The wire from which to get local ID from    //must be 2 or 3 on attiny85 I think
const byte PAO_LED = 26;         //To which pin the <LED> is connected to
const byte PDIO_buspin = 25;

const int TotalLEDs = 1;  //The total amounts of LEDs in the strip
CRGB LEDs[TotalLEDs];
CRGB ColorBoot = CRGB(255, 128, 0);
CRGB ColorGetID = CRGB(255, 0, 0);
CRGB ColorIdle = CRGB(0, 0, 255);
CRGB ColorDispencing = CRGB(0, 255, 0);
byte SlotID = 0;                  //Store the received ID
byte FluidID = 0;                 //Store the fluid of this dispenser
unsigned long lastPulseTime = 0;  //Time of last revieved pulse from SLOT
int DelayAir = 100;               //ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
enum COMMANDS { UNK,
                DISPENSE,
                GETFLUID,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR };
enum COMMAND { DISPENSERUNK,
               DISPENSERSTATUS,
               DISPENSERFLUID,
               DISPENSERSLOT };
PJONSoftwareBitBang bus(DeviceID);  //DeviceID
void setup() {
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);
  pinMode(PDO_ValveFluid, OUTPUT);
  pinMode(PDO_ValveAir, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PDO_ValveFluid, LOW);  //Ensure valve is OFF at start
  digitalWrite(PDO_ValveAir, LOW);    //Ensure valveAir is OFF at start
  FluidID = EEPROM.read(0);           //Read what our fluid is
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
  //Serial.println(".");

  //  if (PrimarySerial.available() >= 3) {// Check if data is available to read
  //    byte SerialSlotID = PrimarySerial.read();
  //    byte Serialcmd1 = PrimarySerial.read();
  //    byte Serialcmd2 = PrimarySerial.read();
  //    while (PrimarySerial.available()) PrimarySerial.read();   //Trash all the other commands if there any
  //    if (SerialSlotID == SlotID){ //If it is for us
  //      byte answer = triggerAction(Serialcmd1, Serialcmd2)
  //      PrimarySerial.print(answer);
  //    }
  //  }
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
    digitalWrite(LED_BUILTIN, HIGH);
    int TimeDelay = payload[1] * 10;
    DispenseStart();
    delay(TimeDelay);
    DispenseStop();
    byte BusSend[] = { DeviceID, DISPENSERSTATUS, true };  //Tell the Primary we have succes
    bus.send(0x00, &BusSend, sizeof(BusSend));             //Send 0x01 (success) to 0x00 (Primary)
    digitalWrite(LED_BUILTIN, LOW);                        //Turn off LED
  }
}
byte triggerAction(byte cmd1, byte cmd2) {
  /*
    DISPENSE, Time_in_ms*10.   
    GETFLUID, IGNORED.         
    CHANGEFLUID, New_ID        
    CHANGEDELAY, delay_in_ms between air and fluid   
    CHANGECOLOR, RRGGBBMM (2 bytes for RGB, where 00=off. MM=01=rainbow)  
  */
  byte r, g, b, m;
  Serial.print("Action" + String(cmd1) + "triggered with variable " + String(cmd2));
  switch (cmd1) {
    case DISPENSE:
      DispenseStart();
      delay(cmd2 * 2);
      DispenseStop();
      break;
    case GETFLUID:
      return FluidID;
    case CHANGEFLUID:
      FluidID = cmd2;
      b = map(b, 0, 3, 0, 255);  // Map the 2-bit value to the range 0-255
      fill_solid(&(LEDs[0]), TotalLEDs, CRGB(r, g, b));
      m = cmd2 & 0x03;  // Mask with 0x03 (00000011)
      if (m == 0b01) {
      }
    //TODO, we can define color modes
    //m 00 = just default RGB,
    //m 01 = rainbow mode
    //m 1x = reserved for other modesDelayAir
    default:
      return 4;
      break;
  }
  return 200;
}
void DispenseStart() {
  Serial.println("DispenseStart");
  fill_solid(&(LEDs[0]), TotalLEDs, ColorDispencing);
  digitalWrite(PDO_ValveAir, HIGH);
  delay(DelayAir);
  digitalWrite(PDO_ValveFluid, HIGH);
  FastLED.show();
}
void DispenseStop() {
  Serial.println("DispenseStop");
  digitalWrite(PDO_ValveAir, LOW);
  digitalWrite(PDO_ValveFluid, LOW);
  fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
  FastLED.show();
}
void CheckAndGetSlotID() {
  if (SlotID == 0) {
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
      SlotID = ID1;
      if (LEDs[0] != ColorDispencing) {  //Do not overwrite ColorDispencing
        fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
        FastLED.show();
      }
      Serial.println("SlotID recieved, I am " + String(SlotID));
    } else {
      Serial.println("Error in SlotID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
    }
  }
}
byte GetSlotID() {
  Serial.println(String(millis()) + "GetSlotID");
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