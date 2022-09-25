/*
  Program written by JelleWho https://github.com/jellewie
  Board: https://dl.espressif.com/dl/package_esp32_index.json
*/
//==============================================================//Note spacer
#if !defined(ESP32)
#error "Please check if the 'DOIT ESP32 DEVKIT V1' board is selected, which can be downloaded at https://dl.espressif.com/dl/package_esp32_index.json"
#endif

#include "WiFiManagerBefore.h"                                  //Define what options to use/include or to hook into WiFiManager
#include "WiFiManager/WiFiManager.h"                            //Includes <WiFi> and <WebServer.h> and setups up 'WebServer server(80)' if needed
//#include FastLED>
#include <AccelStepper.h>                                       //Make sure to install AccelStepper V1.61.0(+) manually //https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a68942c66e78fb7f7b5f0cdade6eb7f06
#include <Wire.h>
#include <LiquidCrystal_I2C.h>                                  //Make sure to install LiquidCrystal_I2C V1.1.2(+) manually https://github.com/johnrickman/LiquidCrystal_I2C/blob/master/LiquidCrystal_I2C.cpp
#include <FastLED.h>                                            //Include the libary FastLED (If you get a error here, make sure it's installed!)
//==============================================================
//Very custom user settings
//==============================================================
const byte PDO_Step_enable = 16;
const byte PAO_LED = 13;                                        //To which pin the <LED strip> is connected to
const byte PDO_X_Dir = 23;
const byte PDO_Y_Dir = 17;
const byte PDO_Z_Dir = 18;
const byte PDO_X_Step = 4;
const byte PDO_Y_Step = 12;
const byte PDO_Z_Step = 19;
const byte PDI_X_Ref = 5;                                       //LOW = TRIGGERED
const byte PDI_Y_Ref = 27;
const byte PDI_Z_Ref = 15;
const byte PDI_S = 39;
const byte PDO_Pump[] = {32, 33, 25, 26};
const int TotalLEDs = 100;                                      //The total amounts of LEDs in the strip
CRGB ColorHoming     = CRGB(  0,   0, 255);
CRGB ColorHomeFail   = CRGB(255,   0,   0);
CRGB ColorMoveBase   = CRGB(  0,   0,   0);
CRGB ColorMoveActive = CRGB(  0, 255,   0);
//==============================================================
//Soft settings (can be changed with the interface)
//==============================================================
byte ShotDispenserML = 40;
byte HomeMAXSpeed = 200;
unsigned int MotorMAXSpeed = 5500;
unsigned int MotorMAXAccel = 3000;
unsigned int BedSize_X = 23950;
unsigned int BedSize_Y = 7100;
unsigned int BedSize_Z = 5000;
unsigned int Manual_X = BedSize_X;
unsigned int Manual_Y = BedSize_Y;
unsigned int HomedistanceBounce = 200;
unsigned int HomedistanceBounceZ = HomedistanceBounce * 4;
unsigned int MaxGlassSize = 300;
unsigned int DisableSteppersAfterIdleS = 60;
byte MaxBrightness = 128;
int SaveEEPROMinSeconds = -1;
int DisableSteppersinSeconds = -1;
bool Homed = false;
//==============================================================
//End of settings
//==============================================================
bool UpdateLEDs = false;
byte Pump_Amount = sizeof(PDO_Pump) / sizeof(PDO_Pump[0]);      //Why filling this in if we can automate that? :)
const byte Dispensers_Amount = 20;                              //Only 20 are saved in the WiFiManager!!
AccelStepper Stepper_X(AccelStepper::DRIVER, PDO_X_Step, PDO_X_Dir);
AccelStepper Stepper_Y(AccelStepper::DRIVER, PDO_Y_Step, PDO_Y_Dir);
AccelStepper Stepper_Z(AccelStepper::DRIVER, PDO_Z_Step, PDO_Z_Dir);
LiquidCrystal_I2C lcd(0x27, 20, 4);                             //Set the LCD address to 0x27 for a 20 chars and 2 line display
CRGB LEDs[TotalLEDs];
#include "Data.h"
#include "Functions.h"
#include "WiFiManagerLater.h"                                   //Define options of WiFiManager (can also be done before), but WiFiManager can also be called here (example for DoRequest included here)

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  LcdPrint("Booting", " ");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  //===========================================================================
  //Init LED and let them shortly blink
  //===========================================================================
  pinMode(PAO_LED, OUTPUT);
  FastLED.addLeds<WS2812B, PAO_LED, GRB>(LEDs, TotalLEDs);
  FastLED.setBrightness(1);                                     //Set start brightness to be amost off
  for (int i = 255; i >= 0; i = i - 255) {                      //Blink on boot
    fill_solid(&(LEDs[0]), TotalLEDs, CRGB(i, i, i));
    FastLED.show();                                             //Update
    FastLED.delay(1);
  }
  FastLED.setBrightness(MaxBrightness);                         //Set brightness
  //===========================================================================
  pinMode(PDO_Step_enable, OUTPUT);
  DisableSteppers();
  pinMode(PDO_X_Dir, OUTPUT);
  pinMode(PDO_Y_Dir, OUTPUT);
  pinMode(PDO_Z_Dir, OUTPUT);
  pinMode(PDO_X_Step, OUTPUT);
  pinMode(PDO_Y_Step, OUTPUT);
  pinMode(PDO_Z_Step, OUTPUT);
  pinMode(PDI_X_Ref, INPUT_PULLUP);
  pinMode(PDI_Y_Ref, INPUT_PULLUP);
  pinMode(PDI_Z_Ref, INPUT_PULLUP);
  pinMode(PDI_S, INPUT_PULLUP);
  for (byte i = 0; i < Pump_Amount; i++) {
    pinMode(PDO_Pump[i], OUTPUT);
    digitalWrite(PDO_Pump[i], LOW);
  }
  Stepper_X.setPinsInverted(false, true, false, false, false); //stepInvert, directionInvert, pin3Invert, pin4Invert, enableInvert
  Stepper_Y.setPinsInverted(false, true, false, false, false);
  Stepper_Z.setPinsInverted(false, true, false, false, false);
  Stepper_X.setMaxSpeed(MotorMAXSpeed);
  Stepper_Y.setMaxSpeed(MotorMAXSpeed);
  Stepper_Z.setMaxSpeed(MotorMAXSpeed);
  Stepper_X.setAcceleration(MotorMAXAccel);
  Stepper_Y.setAcceleration(MotorMAXAccel);
  Stepper_Z.setAcceleration(MotorMAXAccel);
  //===========================================================================
  //Set up all server UrlRequest stuff
  //===========================================================================
  server.on("/",            handle_OnConnect);                  //Call the 'handleRoot' function when a client requests URL "/"
  server.on("/set",         handle_Set);
  server.on("/get",         handle_Get);
  server.on("/info",        handle_Info);
  server.on("/reset",       handle_Reset);
  server.onNotFound(        handle_NotFound);                   //When a client requests an unknown URI
  //server.on("/url", CodeToCall);                              //Example of a url to function caller, These must be declaired before "WiFiManager.Start()"
  LcdPrint("", "WiFi connecting");
  byte Answer = WiFiManager.Start();                            //Run the wifi startup (and save results)
  WiFiManager.OTA_Enabled = true;                               //(runtime) Turn off/on OTA
  WiFiManager.EnableSetup(true);                                //(runtime) Enable the settings, only enabled in APmode by default
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
  EVERY_N_MILLISECONDS(40) {
    LED_Rainbow(0, TotalLEDs, 255 / TotalLEDs); //Show a rainbow to sinal we are done and IDLE
    UpdateLED(true);
  }
}

void MakeCocktail(Drink Mix) {
  LcdPrint("Mixing cocktail", Mix.Name);
  DisableSteppersinSeconds = -1;                               //Make sure the steppers do not auto disable
  if (!Homed) {
    if (!Home(true, true, true)) {
      LcdPrint("Mix not started", "Homing failed");
      return;
    }
  }
  for (byte i = 0; i < 8; i++) {                                //For each Ingredient
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
  MoveTo(Manual_X, Manual_Y, BedSize_Z);
  LcdPrint("Mixed cocktail", Mix.Name);
  FastLED.clear();
  UpdateLED(true);
}
void GetIngredient(Ingredient IN) {
  Serial.println("GetIngredient: ID=" + String(IN.ID) + " Action=" + IN.Action + " ml=" + String(IN.ml) + " from dispenser=" + String(GetDispenserID(IN.ID)));
  if (IN.Action != "") {
    LightSection(Manual_X);
    if (WaitForUser("Waiting on user", String(IN.Action))) return;
  }
  byte DispenserID = GetDispenserID(IN.ID);
  if (DispenserID != 255) {
    LightSection(Dispensers[DispenserID].LocationX);
    switch (Dispensers[DispenserID].Type) {
      case SHOTDISPENSER: {
          Serial.println("GetIngredient SHOTDispenser IN.ml=" + String(IN.ml) + " ShotDispenserML=" + String(ShotDispenserML));
          MoveTo(Dispensers[DispenserID].LocationX, Dispensers[DispenserID].LocationY);
          int mlToDo = IN.ml;
          float DoDelay = 0;
          byte DoAmount = ceil((float)IN.ml / ShotDispenserML);
          for (byte i = 1; i <= DoAmount; i++) {
            Stepper_Z.moveTo(Dispensers[DispenserID].LocationZ);
            while (Stepper_Z.run()) yield();
            //Do some funny stuff so we can try doing half dispensing
            DoDelay = Dispensers[DispenserID].TimeMSML * (mlToDo > ShotDispenserML ? ShotDispenserML : mlToDo);
            mlToDo -= ShotDispenserML;
            MyDelay(DoDelay);
            Stepper_Z.moveTo(0);
            while (Stepper_Z.run()) yield();
            MyDelay(Dispensers[DispenserID].TimeMSoff);
          }
        } break;
      case PUMP: {
          Serial.println("GetIngredient from Pump ID=" + String(Dispensers[DispenserID].LocationZ));
          if (Dispensers[DispenserID].LocationZ <= Pump_Amount) {
            MoveTo(Dispensers[DispenserID].LocationX, Dispensers[DispenserID].LocationY);
            digitalWrite(PDO_Pump[Dispensers[DispenserID].LocationZ], HIGH);
            MyDelay(Dispensers[DispenserID].TimeMSML * IN.ml);
            digitalWrite(PDO_Pump[Dispensers[DispenserID].LocationZ], LOW);
            MyDelay(Dispensers[DispenserID].TimeMSoff);
          } else {
            if (WaitForUser("UNK pump ID", "LocationZ=" + String(Dispensers[DispenserID].LocationZ))) return;
          }
        }
        break;
      default:
        if (WaitForUser("UNK Dispenser type", String(Dispensers[DispenserID].Type))) return;
        break;
    }
  }
}
void LightSection(long LocationX) {
  byte Len = 15;
  float LEDPos = (LocationX * TotalLEDs ) / (BedSize_X);
  LEDPos = LEDPos - Len < 0 ? 0 : LEDPos - Len;
  LEDPos = LEDPos + Len > TotalLEDs ? LEDPos - Len : LEDPos;
  LED_Fill(0, TotalLEDs, ColorMoveBase);              //Set base color
  LED_Fill(LEDPos, Len, ColorMoveActive);
  UpdateLED(true);
}
