/*
  Program written by JelleWho https://github.com/jellewie
  Board: https://dl.espressif.com/dl/package_esp32_index.json ESP32C3
  DISPENSER
*/
#ifndef ESP32
#error "Please select ESP32 as a board."
#endif
#include <FastLED.h>  //Include the libary FastLED (If you get a error here, make sure it's installed!)
#include <EEPROM.h>
#include <PJONSoftwareBitBang.h>
//#define SWBB_MODE 1  //1 should be default, 1=1.97kB/s //https://github.com/gioblu/PJON/blob/master/src/strategies/SoftwareBitBang/README.md#performance
//#define SWBB_MAX_ATTEMPTS 20;	//Maximum transmission attempts	Numeric value (20 by default)
const uint8_t PDI_Button = 2;      //Pull down to trigger
const uint8_t PDO_ValveFluid = 4;  //LOW=OFF
const uint8_t PDO_ValveAir = 3;    //LOW=OFF
#define LED_BUILTIN 8
const uint8_t PDI_SLOT_TXRX = 0;                    //The wire from which to get local ID from
const uint8_t PAO_LED = 1;                          //To which pin the <LED> is connected to
const uint8_t PDIO_buspin = 20;                     //must be 12 or 25 for PJONSoftwareBitBang
const uint8_t PDI_IDBit1 = 6;                       //bit of hardware ID 0b00000001 = 1
const uint8_t PDI_IDBit2 = 7;                       //bit of hardware ID 0b00000010 = 2
const uint8_t PDI_IDBit3 = 8;                       //bit of hardware ID 0b00000100 = 4
const uint8_t PDI_IDBit4 = 10;                      //bit of hardware ID 0b00001000 = 8
const uint8_t PAI_IDBits5to8 = A5;                  //bit of hardware ID 0b11110000 = 16,32,64,128.
const uint8_t PrimaryID = 254;                      //Used to request adoption
const uint8_t TotalLEDs = 1;                        //The total amounts of LEDs in the strip
const uint8_t ManualDispenceML = 15;                //everytime you press the manual button, dispence this amount of ml
CRGB LEDs[TotalLEDs];                               //Array with our status LED
CRGB ColorBoot = CRGB(255, 128, 0);                 //While starting up
CRGB ColorGetID = CRGB(255, 0, 0);                  //While trying to get an IP
CRGB ColorWaitForAdoptionIdle = CRGB(255, 255, 0);  //While waiting for Primary to adopt us
CRGB ColorIdle = CRGB(0, 0, 255);                   //While idle/normal state
CRGB ColorDispencing = CRGB(0, 255, 0);             //While dispensing
bool ImAdopted = false;                             //If the primary has seen this dispenser yet
bool LEDrainbow = false;                            //use to enable rainbow led mode
struct Settings {
  uint8_t IngredientID = 1;    //Default Store the fluid of this dispenser
  uint8_t TimeMSML = 40;       //ms to let 1 ml go, for example it takes 12s to do 300ml, thats about 40 milliseconds per milliliter
  uint8_t DelayAir = 0;        //ms to let the air valve open before the fluid valve, to get rid of pressure buildup in the bottle
  uint16_t FluidLevel = 1000;  //mm of fluid in the bottle
};
Settings dispenserSettings;  //Create a variable of type Settings
enum COMMANDS { DONTREPLY,
                ADOPT,
                DISPENSE,
                CALIBRATEMSPERML,
                CHANGEFLUID,
                CHANGEDELAY,
                CHANGECOLOR,
                DISPENSERSTATUS,
                CHANGEFLUIDLEVEL,
                DONE
};
PJONSoftwareBitBang bus;  //DeviceID = PJON_NOT_ASSIGNED
void setup() {
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);
  fill_solid(&(LEDs[0]), TotalLEDs, ColorBoot);
  FastLED.show();
  Serial.begin(115200);
  pinMode(PDI_Button, INPUT_PULLUP);
  pinMode(PDO_ValveFluid, OUTPUT);
  pinMode(PDO_ValveAir, OUTPUT);
  digitalWrite(PDO_ValveFluid, LOW);  //Ensure valve is OFF at start
  digitalWrite(PDO_ValveAir, LOW);    //Ensure valveAir is OFF at start
  LoadSettings();
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
  delay(250);  //Wait for the connection to be made stably, esp_reset_reason() == ESP_RST_POWERON is power on by power/button
}
void loop() {
  CheckAndGetSlotID();                          //Try and get the SLOT ID
  bool button_state = digitalRead(PDI_Button);  //read manual button
  static bool OLD_button_state = button_state;  //Save old state to see changes
  static uint32_t pressStartTime = 0;           //To store the time we started pressing the button in
  if (button_state != OLD_button_state) {       //Only update if button state changes
    OLD_button_state = button_state;            //remember the new state as the old one
    if (button_state == LOW) {                  //Button just pressed
      pressStartTime = millis();                //Remember when we started with dispensing
      DispenseStart();
    } else {  //Button just released
      uint32_t pressedDuration = millis() - pressStartTime;
      uint32_t MinimumDuration = ManualDispenceML * dispenserSettings.TimeMSML;
      if (pressedDuration < MinimumDuration)       //If we pressed shorter than ManualDispenceML
        delay(MinimumDuration - pressedDuration);  //If released before 1 manual unit dispensed, wait to finish 15ml
      DispenseStop();
    }
  }
  bus.receive(50000);  //Receive messages
  bus.update();        //Handle bus updates
  LEDloop(false);      //Do the LED update loop if needed, this is for rainbow.
}
void LoadSettings() {
  EEPROM.begin(16);  //Initialize EEPROM (needed for ESP32)
  dispenserSettings.IngredientID = EEPROM.read(0);
  dispenserSettings.DelayAir = EEPROM.read(1);
  dispenserSettings.TimeMSML = EEPROM.read(2);
  for (uint8_t i = 0; i < 3; i++)
    Serial.println("LoadedSetting " + String(i) + "=" + EEPROM.read(i));
}
void SaveSettings() {
  EEPROM.write(0, dispenserSettings.IngredientID);
  EEPROM.write(1, dispenserSettings.DelayAir);
  EEPROM.write(2, dispenserSettings.TimeMSML);
  EEPROM.commit();  //Commit changes (needed for ESP32)
}
void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if (code == PJON_CONNECTION_LOST)
    ImAdopted = false;
  if (code == PJON_CONNECTION_LOST)
    Serial.println("Connection with device ID " + String(bus.packets[data].content[0]) + " is lost.");
  if (code == PJON_PACKETS_BUFFER_FULL)
    Serial.println("Packet buffer is full, has now a length of " + String(data) + " Possible wrong bus configuration!");
  if (code == PJON_CONTENT_TOO_LONG)
    Serial.println("Content is too long, length: " + String(data));
}
void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  Serial.print("Received: ");
  for (uint16_t i = 0; i < length; i++)
    Serial.print(payload[i] + String(" "));
  Serial.println();
  /*
    1=ADOPT, IGNORED
    2=DISPENSE, ml to dispense
    3=CALIBRATEMSPERML, TimeMSML
    4=CHANGEFLUID, New_Fluid_ID
    5=CHANGEDELAY, Time_in_ms*5 between air and fluid
    6=CHANGECOLOR, RRGGBBMM MM=00 = Auto
                            MM=01 = RGB mode,
                            MM=10 = rainbow mode
                            MM=11 = reserved for other modes
    7=CHANGEFLUIDLEVEL, estimated ml left in the bottle. used to compensate for head pressure
  */
  switch (payload[0]) {
    case ADOPT:
      {
        ImAdopted = true;
        LEDloop(true);
        uint8_t BusSend[] = { DISPENSERSTATUS, dispenserSettings.IngredientID, dispenserSettings.TimeMSML, dispenserSettings.DelayAir, dispenserSettings.FluidLevel };  //Reply back we have completed
        uint16_t result = bus.reply(&BusSend, sizeof(BusSend));                                                                                                         //Send success to Primary
      }
    case DISPENSE:
      {
        DispenseStart();
        uint16_t DelayCompensated = (400 - (dispenserSettings.FluidLevel * 0.0727 + 142.9)) / 200 * payload[1]* dispenserSettings.TimeMSML;
        Serial.println("Dispensing " + String(payload[1]) + "ml, raw delay=" + String(payload[1] * dispenserSettings.TimeMSML) + " compensated delay=" + String(DelayCompensated));
        delay(DelayCompensated);
        DispenseStop();
        Serial.println("Done, sending feedback to Primary");
        uint8_t BusSend[] = { DONE };  //Tell primary we have completed it's command
        bus.send(PrimaryID, &BusSend, sizeof(BusSend));
        break;
      }
    case CALIBRATEMSPERML:
      dispenserSettings.TimeMSML = payload[1];
      SaveSettings();
      break;
    case CHANGEFLUID:
      if (payload[1] != DONTREPLY) {
        dispenserSettings.IngredientID = payload[1];
        SaveSettings();
      }
      break;
    case CHANGEDELAY:
      dispenserSettings.DelayAir = payload[1];
      SaveSettings();
      break;
    case CHANGECOLOR:
      {
        uint8_t r, g, b, m;
        r = (payload[1] >> 6) & 0b11;  //Extract bits 7-6 (RR)
        g = (payload[1] >> 4) & 0b11;  //Extract bits 5-4 (GG)
        b = (payload[1] >> 2) & 0b11;  //Extract bits 3-2 (BB)
        r = map(r, 0, 3, 0, 255);      //Map the 2-bit value to the range 0-255
        g = map(g, 0, 3, 0, 255);      //Map the 2-bit value to the range 0-255
        b = map(b, 0, 3, 0, 255);      //Map the 2-bit value to the range 0-255
        m = payload[1] & 0x03;         //Mask with 0x03 (00000011)
        if (m == 0) {
          LEDrainbow = false;
          LEDloop(true);
        }
        if (m == 1) {
          fill_solid(&(LEDs[0]), TotalLEDs, CRGB(r, g, b));
          FastLED.show();
        }
        if (m == 2)
          if (LEDs[0] == ColorIdle or LEDrainbow == 1) {  //Do not overwrite other modes OR if we want to sync the rainbow
            LEDrainbow = true;                            //Set rainbow mode
            LEDloop(true);
          }
        Serial.println("r=" + String(r) + " g=" + String(g) + " b=" + String(b) + " m=" + String(m));
        break;
      }
    case CHANGEFLUIDLEVEL:
      {
        dispenserSettings.FluidLevel = payload[1];
        break;
      }
  }
}
void DispenseStart() {
  LEDrainbow = false;
  fill_solid(&(LEDs[0]), TotalLEDs, ColorDispencing);
  FastLED.show();
  digitalWrite(PDO_ValveAir, HIGH);
  delay(dispenserSettings.DelayAir * 5);
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
    static unsigned long lastUpdate = 0;  //Every 40 milliseconds, increment the hue to create the rainbow effect
    if (millis() - lastUpdate >= 40) {
      lastUpdate = millis();
      gHue++;
      fill_rainbow(&LEDs[0], 1, gHue, 1);
      FastLED.show();
    }
  } else {
    if (init) {
      if (ImAdopted) {
        fill_solid(&(LEDs[0]), TotalLEDs, ColorIdle);
      } else {
        if (bus.device_id() == 0)
          fill_solid(&(LEDs[0]), TotalLEDs, ColorGetID);  //No ID known
        else
          fill_solid(&(LEDs[0]), TotalLEDs, ColorWaitForAdoptionIdle);  //Not yet adopted
      }
      FastLED.show();
    }
  }
}
void CheckAndGetSlotID() {
  //Check if ID is set by hardware, else we get it from the slot
  if (bus.device_id() == PJON_NOT_ASSIGNED) {
    if (LEDs[0] != ColorGetID and LEDs[0] != ColorDispencing) {  //If not yet desired color, but do not overwrite ColorDispencing
      fill_solid(&(LEDs[0]), TotalLEDs, ColorGetID);
      FastLED.show();
    }
    pinMode(PDI_IDBit1, INPUT_PULLUP);
    pinMode(PDI_IDBit2, INPUT_PULLUP);
    pinMode(PDI_IDBit3, INPUT_PULLUP);
    pinMode(PDI_IDBit4, INPUT_PULLUP);
    pinMode(PAI_IDBits5to8, INPUT_PULLUP);
    uint8_t digitalID = (!digitalRead(PDI_IDBit4) << 3) | (!digitalRead(PDI_IDBit3) << 2) | (!digitalRead(PDI_IDBit2) << 1) | !digitalRead(PDI_IDBit1);
    uint8_t analogID = analogRead(PAI_IDBits5to8) / 256;  //Divide by 256 to get 16 levels
    uint8_t deviceID = (analogID << 4) | digitalID;
    if (deviceID != 0) {  //If no ID has been defined in hardware
      Serial.println("SlotID defined in hardware, I am " + String(deviceID));
    } else {
      uint8_t ID1 = GetSlotID();
      delayMicroseconds(100);  //must be the same or higher than DelayAfterSend-(lastPulseTimeTimeout/2)
      uint8_t ID2 = GetSlotID();
      delayMicroseconds(100);  //must be the same or higher than DelayAfterSend-(lastPulseTimeTimeout/2)
      uint8_t ID3 = GetSlotID();

      pinMode(LED_BUILTIN, OUTPUT);  //HIGH = OFF
      static const uint16_t BlinkDelayOn = 100;
      static const uint16_t BlinkDelayOff = 1000;
      bool WeBlinked = false;
      if (ID1 + ID2 + ID3 == 0) {  //If they are all wrong
        digitalWrite(LED_BUILTIN, LOW);
        delay(BlinkDelayOn * 5);
        digitalWrite(LED_BUILTIN, HIGH);
      } else {  //Show who failed
        if (ID1 == 0) {
          WeBlinked = true;
          for (int i = 0; i < 1; i++) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(BlinkDelayOn);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(BlinkDelayOn);
          }
        }
        if (ID2 == 0 or (ID2 != ID1 and ID2 != ID3)) {
          if (WeBlinked) delay(BlinkDelayOff);
          WeBlinked = true;
          for (int i = 0; i < 2; i++) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(BlinkDelayOn);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(BlinkDelayOn);
          }
        }
        if (ID3 == 0 or (ID3 != ID1 and ID3 != ID2)) {
          if (WeBlinked) delay(BlinkDelayOff);
          for (int i = 0; i < 3; i++) {
            digitalWrite(LED_BUILTIN, LOW);
            delay(BlinkDelayOn);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(BlinkDelayOn);
          }
        }
      }
      digitalWrite(LED_BUILTIN, HIGH);  //HIGH = OFF

      if (ID1 > 0 && (ID1 == ID2) or (ID1 == ID3)) {  //Two measurements need to agree
        deviceID = ID1;
      } else if (ID2 > 0 && ID2 == ID3)
        deviceID = ID2;
      else
        Serial.println("Error in SlotID recieved, ID1=" + String(ID1) + " ID2=" + String(ID2) + " ID3=" + String(ID3));
    }
    bus.set_id(deviceID);
    if (LEDs[0] != ColorDispencing)  //Do not overwrite ColorDispencing
      LEDloop(true);

    if (bus.device_id() != 0) {
      uint8_t BusSend[] = { ADOPT, bus.device_id() };  //Ask Primary for us to be adopted
      bus.send(PrimaryID, &BusSend, sizeof(BusSend));
      Serial.println("SlotID recieved, I am " + String(bus.device_id()));
    }
    else {
      Serial.println("Could not recieve Slot ID");
    }
  }
}
uint8_t GetSlotID() {
  uint8_t pulseCount = 0;  //Amount of pulses counted from SLOT
  bool OLD_PIN_state = 1;
  const static uint16_t lastPulseTimeTimeout = 75;
  pinMode(PDI_SLOT_TXRX, OUTPUT);
  digitalWrite(PDI_SLOT_TXRX, LOW);
  delayMicroseconds(5);  //Give the SLOT time to notice us
  pinMode(PDI_SLOT_TXRX, INPUT);
  unsigned long lastPulseTime;
  lastPulseTime = micros();
  while (micros() - lastPulseTime < lastPulseTimeTimeout) {  //Do NOT put Serial in this while loop. It will be to slow to count the pulses
    bool PIN_state = digitalRead(PDI_SLOT_TXRX);
    if (PIN_state != OLD_PIN_state) {  //Only update if state changes
      OLD_PIN_state = PIN_state;
      if (PIN_state == HIGH) {  //Only count if the line goes HIGH
        pulseCount++;
        if (pulseCount == 255)
          return 0;
      }
      lastPulseTime = micros();  //Reset timeout on pulse detection
    }
  }
  //pinMode(PDI_SLOT_TXRX, OUTPUT);
  digitalWrite(PDI_SLOT_TXRX, HIGH);
  return pulseCount;
}
