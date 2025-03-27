/*
  Program written by JelleWho https://github.com/jellewie
  Board: https://dl.espressif.com/dl/package_esp32_index.json
*/
#if !defined(ESP32)
#error "Please check if the 'DOIT ESP32 DEVKIT V1' board is selected, which can be downloaded at https://dl.espressif.com/dl/package_esp32_index.json"
#endif
#include <WiFi.h>                     //Arduino IDe 2.3.4 requires this here for some reason
#include "WiFiManagerBefore.h"        //Define what options to use/include or to hook into WiFiManager
#include "WiFiManager/WiFiManager.h"  //Includes <WiFi> and <WebServer.h> and setups up 'WebServer server(80)' if needed
#include <AccelStepper.h>             //Make sure to install AccelStepper V1.61.0(+) manually //https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a68942c66e78fb7f7b5f0cdade6eb7f06
#include <Wire.h>                     //used by LiquidCrystal_I2C
#include <LiquidCrystal_I2C.h>        //Make sure to install LiquidCrystal_I2C V1.1.2(+) manually https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp
#include <FastLED.h>                  //Include the libary FastLED (If you get a error here, make sure it's installed!)
#include <PJONSoftwareBitBang.h>
//==============================================================
//Very custom user settings
//==============================================================
const uint8_t PDO_Step_enable = 16;
const uint8_t PAO_LED = 13;  //To which pin the <LED strip> is connected to
const uint8_t PDO_X_Dir = 23;
const uint8_t PDO_Y_Dir = 17;
const uint8_t PDO_X_Step = 4;
const uint8_t PDO_Y_Step = 12;
const uint8_t PDI_X_Ref = 5;  //LOW = TRIGGERED
const uint8_t PDI_Y_Ref = 27;
const uint8_t PDI_S = 39;
const uint8_t PDIO_buspin = 25;
const uint16_t TotalLEDs = 100;  //The total amounts of LEDs in the strip
const uint16_t DispenserHeartbeat = 5000; //time in ms to check if all dispensers are still there
CRGB ColorBoot = CRGB(255, 128, 0);
CRGB ColorHoming = CRGB(0, 0, 255);
CRGB ColorHomeFail = CRGB(255, 0, 0);
CRGB ColorMoveBase = CRGB(0, 0, 0);
CRGB ColorMoveActive = CRGB(0, 255, 0);
//==============================================================
//Soft settings (can be changed with the interface)
//==============================================================
uint8_t HomeMAXSpeed = 200;
bool Running = false;
uint16_t MotorMAXSpeed = 5500;
uint16_t MotorMAXAccel = 3000;
uint16_t BedSize_X = 23950;
uint16_t BedSize_Y = 7100;
uint16_t Manual_X = BedSize_X;
uint16_t Manual_Y = BedSize_Y;
uint16_t HomedistanceBounce = 200;
uint16_t MaxGlassSize = 300;
uint16_t DisableSteppersAfterIdleS = 60;
uint8_t MaxBrightness = 128;
int16_t SaveEEPROMinSeconds = -1;
int16_t DisableSteppersinSeconds = -1;
bool Homed = false;
//==============================================================
//End of settings
//==============================================================
bool UpdateLEDs = false;
const uint8_t Dispensers_Amount = 20 + 1;  //Only 20 are saved in the WiFiManager!! 00 is reserved/invalid_ID so add 1 here
bool SlotConnected[Dispensers_Amount];
AccelStepper Stepper_X(AccelStepper::DRIVER, PDO_X_Step, PDO_X_Dir);
AccelStepper Stepper_Y(AccelStepper::DRIVER, PDO_Y_Step, PDO_Y_Dir);
LiquidCrystal_I2C lcd(0x27, 20, 4);  //Set the LCD address to 0x27 for a 20 chars and 2 line display
const uint8_t PrimaryID = 254;
PJONSoftwareBitBang bus(PrimaryID);  //Master Device ID
CRGB LEDs[TotalLEDs];
#include "data.h"
#include "functions.h"
#include "dispensers.h"
#include "WiFiManagerLater.h"  //Define options of WiFiManager (can also be done before), but WiFiManager can also be called here (example for DoRequest included here)

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  LcdPrint("Booting", " ");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  //===========================================================================
  //Init LED and set the boot color
  //===========================================================================
  pinMode(PAO_LED, OUTPUT);
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);
  FastLED.setBrightness(MaxBrightness);  //Set brightness
  LED_Fill(0, TotalLEDs, ColorBoot);
  FastLED.show();  //Update
  //===========================================================================
  pinMode(PDO_Step_enable, OUTPUT);
  DisableSteppers();
  pinMode(PDO_X_Dir, OUTPUT);
  pinMode(PDO_Y_Dir, OUTPUT);
  pinMode(PDO_X_Step, OUTPUT);
  pinMode(PDO_Y_Step, OUTPUT);
  pinMode(PDI_X_Ref, INPUT_PULLUP);
  pinMode(PDI_Y_Ref, INPUT_PULLUP);
  pinMode(PDI_S, INPUT_PULLUP);
  Stepper_X.setPinsInverted(false, true, false, false, false);  //stepInvert, directionInvert, pin3Invert, pin4Invert, enableInvert
  Stepper_Y.setPinsInverted(false, true, false, false, false);
  Stepper_X.setMaxSpeed(MotorMAXSpeed);
  Stepper_Y.setMaxSpeed(MotorMAXSpeed);
  Stepper_X.setAcceleration(MotorMAXAccel);
  Stepper_Y.setAcceleration(MotorMAXAccel);
  //===========================================================================
  //Set up all server UrlRequest stuff
  //===========================================================================
  server.on("/", handle_OnConnect);  //Call the 'handleRoot' function when a client requests URL "/"
  server.on("/set", handle_Set);
  server.on("/get", handle_Get);
  server.on("/info", handle_Info);
  server.on("/reset", handle_Reset);
  server.onNotFound(handle_NotFound);  //When a client requests an unknown URI
  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
  bus.strategy.set_pin(PDIO_buspin);
  bus.begin();
  uint8_t Answer = WiFiManager.Start();  //Run the wifi startup (and save results)
  WiFiManager.OTA_Enabled = true;        //(runtime) Turn off/on OTA
  WiFiManager.EnableSetup(true);         //(runtime) Enable the settings, only enabled in APmode by default
  pingAll();                             //Look for all available dispensers
  if (Answer == 1) {
    LcdPrint("Mixer online!", IpAddress2String(WiFi.localIP()));
  } else {
    LcdPrint("WiFi status = " + String(Answer), IpAddress2String(WiFi.localIP()));
  }
  digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
  MyYield();
  server.handleClient();
  static bool HomedLast = Homed;
  if (Homed != HomedLast) {
    HomedLast = Homed;
    BusSend(CHANGECOLOR, 0b00000010);  //Send dispenser LED Rainbow command
  }
  if (Homed) {
    EVERY_N_MILLISECONDS(40) {
      LED_Rainbow(0, TotalLEDs, 255 / TotalLEDs);  //Show a rainbow to sinal we are done and IDLE
      UpdateLED(true);
    }
  }
  static unsigned long LastTime;
  if (TickEveryXms(&LastTime, DispenserHeartbeat))
    pingOnline(); //Heartbeat to all dispensers to check if they are still there
}
void MakeCocktail(Drink Mix) {
  if (Running) return;
  Running = true;
  LcdPrint("Mixing cocktail", Mix.Name);
  BusSend(CHANGECOLOR, 0b00000000);  //Reset dispenser LED color command
  DisableSteppersinSeconds = -1;     //Make sure the steppers do not auto disable
  if (!Homed) {
    if (!Home(true, true)) {
      LcdPrint("Mix not started", "Homing failed");
      return;
    }
  }
  for (uint8_t i = 0; i < 8; i++) {  //For each Ingredient
    MyYield();
    if (Mix.Ingredients[i].ID != 0 or Mix.Ingredients[i].Action != 0) {
      String msg = "";
      if (Mix.Ingredients[i].ID != 0)
        LcdPrint("Getting", IngredientIDtoString(Mix.Ingredients[i].ID));
      else
        LcdPrint("Please add", Mix.Ingredients[i].Action);
      GetIngredient(Mix.Ingredients[i]);
    }
  }
  MoveTo(Manual_X, Manual_Y);
  LcdPrint("Mixed cocktail", Mix.Name);
  FastLED.clear();
  UpdateLED(true);
  Running = false;
}
void GetIngredient(Ingredient IN) {
  Serial.println("GetIngredient: ID=" + String(IN.ID) + " Action=" + IN.Action + " ml=" + String(IN.ml) + " from dispenser=" + String(GetDispenserID(IN.ID)));
  if (IN.Action != "") {
    LightSection(Manual_X);
    if (WaitForUser("Waiting on user", String(IN.Action))) return;
  }
  uint8_t DispenserID = GetDispenserID(IN.ID);
  if (DispenserID != 255) {
    LightSection(Dispensers[DispenserID].LocationX);                               //Turn on the main LED so show where we are going
    MoveTo(Dispensers[DispenserID].LocationX, Dispensers[DispenserID].LocationY);  //Move to the dispenser
    BusSendBlocking(DispenserID, DISPENSE, IN.ml);                                 //Give the dispence command
  }
}
void LightSection(long LocationX) {
  uint8_t Len = 13;
  float LEDPos = (LocationX * TotalLEDs) / (BedSize_X);
  LEDPos = LEDPos - Len < 0 ? 0 : LEDPos - Len;
  LEDPos = LEDPos + Len > TotalLEDs ? LEDPos - Len : LEDPos;
  LED_Fill(0, TotalLEDs, ColorMoveBase);  //Set base color
  LED_Fill(LEDPos, Len, ColorMoveActive);
  UpdateLED(true);
}
