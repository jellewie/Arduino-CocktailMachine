/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
  DISPENSER
*/
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <PJONSoftwareBitBang.h>
#define SWBB_MODE 1  //Should be default, 1=1.97kB/s //https://github.com/gioblu/PJON/blob/master/src/strategies/SoftwareBitBang/README.md#performance
//#define SWBB_MAX_ATTEMPTS 20;	//Maximum transmission attempts	Numeric value (20 by default)

#ifdef ESP32
const uint8_t PDI_Button = 14;      //Pull down to trigger
const uint8_t PDO_ValveFluid = 33;  //LOW=OFF
const uint8_t PDO_ValveAir = 32;    //LOW=OFF
const uint8_t PDI_SLOT_TXRX = 13;   //The wire from which to get local ID from    //must be 2 or 3 on attiny85 I think
const uint8_t PAO_LED = 26;         //To which pin the <LED> is connected to
const uint8_t PDIO_buspin = 25;     //must be 12,25 for ESP32
#else
const uint8_t PDI_Button = 3;      //Pull down button to trigger
const uint8_t PDO_ValveFluid = 2;  //LOW=OFF
const uint8_t PDO_ValveAir = 5;    //LOW=OFF
const uint8_t PDI_SLOT_TXRX = 0;   //The wire from which to get local ID from    //must be 2 or 3 on attiny85 I think
const uint8_t PAO_LED = 4;         //To which pin the <LED> is connected to
const uint8_t PDIO_buspin = 1;     //Must be 1/2 for ATtiny85
#endif
const uint8_t PrimaryID = 254;                                      //Used to request adoption
const uint8_t TotalLEDs = 1;                                        //The total amounts of LEDs in the strip
Adafruit_NeoPixel strip(TotalLEDs, PAO_LED, NEO_GRB + NEO_KHZ800);  //Array with our status LED
const uint32_t ColorBoot = 0xFF8000;                                //While starting up
const uint32_t ColorGetID = 0xFF0000;                               //While trying to get an IP
const uint32_t ColorWaitForAdoptionIdle = 0xFFFF00;                 //While waiting for Primary to adopt us
const uint32_t ColorIdle = 0x0000FF;                                //While idle/normal state
const uint32_t ColorDispencing = 0x00FF00;                          //While dispensing

uint8_t FluidID = 0;              //SAVESETTING Store the fluid of this dispenser
uint8_t MSperML = 40;             //SAVESETTING ms to let 1 ml go, for example it takes 12s to do 300ml, thats about 40 ms per ml
uint8_t DelayAir = 0;             //SAVESETTING ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
unsigned long lastPulseTime = 0;  //Time of last revieved pulse from SLOT
bool ImAdopted = false;           //If the primary has seen this dispenser yet
bool LEDrainbow = false;          //use to enable rainbow led mode
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
#ifdef ESP32
  Serial.begin(115200);
#endif
  pinMode(PDI_Button, INPUT_PULLUP);
  pinMode(PDO_ValveFluid, OUTPUT);
  pinMode(PDO_ValveAir, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(PDO_ValveFluid, LOW);  //Ensure valve is OFF at start
  digitalWrite(PDO_ValveAir, LOW);    //Ensure valveAir is OFF at start
  LoadSettings();
  strip.setPixelColor(0, ColorBoot);  //RED to show we do not have an ID
  strip.begin();
  strip.show();
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
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

  LEDloop(false);  //Do the LED update loop if needed, this is for rainbow.
}
void LoadSettings() {
#ifdef ESP32
  for (uint8_t i = 0; i < 3; i++)
    Serial.println("LoadedSetting " + String(i) + "=" + EEPROM.read(i));
  EEPROM.begin(16);  //Needed for ESP32
#endif
  FluidID = EEPROM.read(0);
  DelayAir = EEPROM.read(1);
  MSperML = EEPROM.read(2);
}
void SaveSettings() {
  EEPROM.write(0, FluidID);
  EEPROM.write(1, DelayAir);
  EEPROM.write(2, MSperML);
#ifdef ESP32
  EEPROM.commit();  //Needed for ESP32
#endif
}
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
#ifdef ESP32
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
#endif
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
#ifdef ESP32
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println();
#endif
  if (length == 2) {
    uint8_t triggerresult = triggerAction(payload[0], payload[1]);
    uint8_t BusSend[] = { DISPENSERSTATUS, triggerresult };   //Reply back we have completed
    uint16_t result2 = bus.reply(&BusSend, sizeof(BusSend));  //Send success Primary
#ifdef ESP32
    Serial.println("called triggerAction with result=" + String(triggerresult));
    if (result2 == PJON_FAIL)
      Serial.print("bus.reply wrong =" + String(result2));
#endif
  }
}
uint8_t triggerAction(uint8_t cmd1, uint8_t cmd2) {
  /*
    1=ADOPT, IGNORED
    2=DISPENSE, ml to dispense
    3=CALIBRATEMSPERML, MSperML
    4=CHANGEFLUID, New_Fluid_ID
    5=CHANGEDELAY, Time_in_ms*5 between air and fluid   
    6=CHANGECOLOR, RRGGBBMM MM=00 = Auto
                            MM=01 = RGB mode,
                            MM=10 = rainbow mode
                            MM=11 = reserved for other modes
  */
  uint8_t r, g, b, m;
  switch (cmd1) {
    case ADOPT:
      ImAdopted = true;
      LEDloop(true);
      return FluidID;
      break;
    case DISPENSE:
      DispenseStart();
      delay(cmd2 * MSperML);
      DispenseStop();
      break;
    case CALIBRATEMSPERML:
      MSperML = cmd2;
      SaveSettings();
      break;
    case CHANGEFLUID:
      FluidID = cmd2;
      SaveSettings();
      break;
    case CHANGEDELAY:
      DelayAir = cmd2;
      SaveSettings();
      break;
    case CHANGECOLOR:
      // r = (cmd2 >> 6) & 0b11;    // Extract bits 7-6 (RR)
      // g = (cmd2 >> 4) & 0b11;    // Extract bits 5-4 (GG)
      // b = (cmd2 >> 2) & 0b11;    // Extract bits 3-2 (BB)
      // r = map(r, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      // g = map(g, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      // b = map(b, 0, 3, 0, 255);  //Map the 2-bit value to the range 0-255
      m = cmd2 & 0x03;  //Mask with 0x03 (00000011)
      if (m == 0) {
        LEDrainbow = false;
        LEDloop(true);
      }
      // if (m == 1) {
      //   strip.setPixelColor(0, strip.Color(r, g, b));
      //   strip.show();
      // }
      if (m == 2)
        if (strip.getPixelColor(0) == ColorIdle or LEDrainbow == 1) {  //Do not overwrite other modes OR if we want to sync the rainbow
          LEDrainbow = true;                                           //Set rainbow mode
          LEDloop(true);
        }
#ifdef ESP32
      Serial.println("r=" + String(r) + " g=" + String(g) + " b=" + String(b) + " m=" + String(m));
#endif
      break;
    default:
      return false;
      break;
  }
  return true;
}
void DispenseStart() {
  LEDrainbow = false;
  strip.setPixelColor(0, ColorDispencing);
  strip.show();
  digitalWrite(PDO_ValveAir, HIGH);
  delay(DelayAir * 5);
  digitalWrite(PDO_ValveFluid, HIGH);
}
void DispenseStop() {
  digitalWrite(PDO_ValveAir, LOW);
  digitalWrite(PDO_ValveFluid, LOW);
  LEDloop(true);
}
void LEDloop(bool init) {
  if (LEDrainbow) {
    static uint8_t gHue;
    if (init)
      gHue = 0;                           //Reset gHue offset
    static unsigned long lastUpdate = 0;  // Every 40 milliseconds, increment the hue to create the rainbow effect
    if (millis() - lastUpdate >= 40) {
      lastUpdate = millis();
      gHue++;  // Increment hue

      // Create rainbow effect by setting color based on the current hue
      uint32_t color = strip.ColorHSV(gHue * 256);  // Get RGB color from the HSV value
      strip.setPixelColor(0, color);                // Set the color to the first LED
      strip.show();                                 // Update the LED strip
    }
  } else {
    if (init) {
      if (ImAdopted) {
        strip.setPixelColor(0, ColorIdle);  // Set LED to red
      } else {
        strip.setPixelColor(0, ColorWaitForAdoptionIdle);
      }
      strip.show();
    }
  }
}
void CheckAndGetSlotID() {
  if (bus.device_id() == PJON_NOT_ASSIGNED) {
    if (strip.getPixelColor(0) != ColorGetID and strip.getPixelColor(0) != ColorDispencing) {  //If not yet desired color, but do not overwrite ColorDispencing
      strip.setPixelColor(0, ColorGetID);
      strip.show();
    }
    delay(1);
    uint8_t ID1 = GetSlotID();
    delay(1);
    uint8_t ID2 = GetSlotID();
    delay(1);
    uint8_t ID3 = GetSlotID();
    if (ID1 > 0 && ID1 == ID2 && ID2 == ID3) {
      bus.set_id(ID1);
      if (strip.getPixelColor(0) != ColorDispencing)  //Do not overwrite ColorDispencing
        LEDloop(true);
      uint8_t BusSend[] = { ADOPT, bus.device_id() };  //Ask Primary for us to be adopted
      uint16_t result = bus.send(PrimaryID, &BusSend, sizeof(BusSend));
#ifdef ESP32
      Serial.println("SlotID recieved, I am " + String(bus.device_id()));
      if (result == PJON_FAIL) Serial.print("bus request fail =" + String(result));
#endif
    }
#ifdef ESP32
    else
      Serial.println("Error in SlotID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
#endif
  }
}
uint8_t GetSlotID() {
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