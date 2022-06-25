/*
  Program written by JelleWho https://github.com/jellewie
  Board: https://dl.espressif.com/dl/package_esp32_index.json


  save dispencer data in EEPROM
  Get AddDispencer code
*/
//==============================================================//Note spacer
#if !defined(ESP32)
#error "Please check if the 'DOIT ESP32 DEVKIT V1' board is selected, which can be downloaded at https://dl.espressif.com/dl/package_esp32_index.json"
#endif

#include "WiFiManagerBefore.h"                                  //Define what options to use/include or to hook into WiFiManager
#include "WiFiManager/WiFiManager.h"                            //Includes <WiFi> and <WebServer.h> and setups up 'WebServer server(80)' if needed
//#include FastLED>
#include <AccelStepper.h>                                       //Make sure to install AccelStepper V1.61.0(+) manually //https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a68942c66e78fb7f7b5f0cdade6eb7f06

const byte PDO_Step_enable = 22;
const byte PDO_X_Dir = 5;
const byte PDO_Y_Dir = 25;
const byte PDO_Z_Dir = 14;
const byte PDO_X_Step = 32;
const byte PDO_Y_Step = 26;
const byte PDO_Z_Step = 12;
const byte PDI_X_Ref = 18;                           //LOW = TRIGGERED
const byte PDI_Y_Ref = 19;
const byte PDI_Z_Ref = 21;
const byte PDO_Pump[] = {33, 27};
int MotorMAXSpeed = 5500;
int MotorMAXAccel = 3000;
int BedSize_X = 26000;
int BedSize_Y = 1000;
int BedSize_Z = 1000;
int Manual_X = 0;
int Manual_Y = 0;
byte ShotDispenserML = 30;
byte HomeMAXSpeed = 200;
int HomedistanceBounce = 400;
bool DisableSteppersAfterMixDone = false;

bool Homed = false;
byte Pump_Amount = sizeof(PDO_Pump) / sizeof(PDO_Pump[0]);      //Why filling this in if we can automate that? :)
const int ErrorLEDHoming = 250;
const byte Dispensers_Amount = 20;                              //Only 20 are saved in the WiFiManager!!
AccelStepper Stepper_X(AccelStepper::DRIVER, PDO_X_Step, PDO_X_Dir);
AccelStepper Stepper_Y(AccelStepper::DRIVER, PDO_Y_Step, PDO_Y_Dir);
AccelStepper Stepper_Z(AccelStepper::DRIVER, PDO_Z_Step, PDO_Z_Dir);

#include "Functions.h"
#include "Data.h"
#include "WiFiManagerLater.h"                                   //Define options of WiFiManager (can also be done before), but WiFiManager can also be called here (example for DoRequest included here)
byte Ingredient_Amount = sizeof(IngredientS) / sizeof(IngredientS[0]);//Why filling this in if we can automate that? :)
byte Drinks_Amount = sizeof(Drinks) / sizeof(Drinks[0]);        //Why filling this in if we can automate that? :)

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PDO_Step_enable, OUTPUT);
  pinMode(PDO_X_Dir, OUTPUT);
  pinMode(PDO_Y_Dir, OUTPUT);
  pinMode(PDO_Z_Dir, OUTPUT);
  pinMode(PDO_X_Step, OUTPUT);
  pinMode(PDO_Y_Step, OUTPUT);
  pinMode(PDO_Z_Step, OUTPUT);
  pinMode(PDI_X_Ref, INPUT_PULLUP);
  pinMode(PDI_Y_Ref, INPUT_PULLUP);
  pinMode(PDI_Z_Ref, INPUT_PULLUP);

  Stepper_X.setPinsInverted(false, true, false, false, false);
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
  byte Answer = WiFiManager.Start();                            //Run the wifi startup (and save results)
  Serial.println("WiFi setup executed with responce code '" + String(Answer) + "' " + IpAddress2String(WiFi.localIP())); //The return codes can be found in "WiFiManager.cpp" in "CWiFiManager::Start("
  WiFiManager.OTA_Enabled = true;                               //(runtime) Turn off/on OTA
  WiFiManager.EnableSetup(true);                                //(runtime) Enable the settings, only enabled in APmode by default
  digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
  WiFiManager.RunServer();                                      //(runtime) Note when using OTA, you need to run the server in your loop
}
void MakeCocktail(Drink Mix) {
  if (!Homed) {
    if (!Home(true, false, false))
      return;
  }

  for (byte i = 0; i < 8; i++) {                                //For each Ingredient
    if (Mix.Ingredients[i].ID != 0 or Mix.Ingredients[i].Action != 0) {
      Serial.println(" == Drink " + String(Mix.Name) + "' Next ingredient ='" + IngredientIDtoString(Mix.Ingredients[i].ID) + "'");
      GetIngredient(Mix.Ingredients[i]);
    }
  }
  MoveTo(Manual_X, Manual_Y);
}
void GetIngredient(Ingredient IN) {
  Serial.println("GetIngredient: Name=" + String(IN.ID) + " Action=" + IN.Action + " ml=" + String(IN.ml));
  if (IN.Action != "") {
    MoveTo(Manual_X, Manual_Y);
    Serial.println("TODO, Wait for user confirmation code, msg = '" + String(IN.Action) + "'");
    MyDelay(2500);
  }
  byte DispenserID = GetDispenserID(IN.ID);
  if (DispenserID != 0) {
    switch (Dispensers[DispenserID].Type) {
      case SHOTDispenser: {
          Serial.println("GetIngredient SHOTDispenser");
          MoveTo(Dispensers[DispenserID].LocationX, Dispensers[DispenserID].LocationY);
          int mlToDo = IN.ml;
          float DoDelay = 0;
          byte DoAmount = ceil(IN.ml / ShotDispenserML);
          for (byte i = 1; i <= DoAmount; i++) {
            Stepper_Z.moveTo(Dispensers[DispenserID].LocationZ);
            while (Stepper_Z.run()) yield();

            //Do some funny stuff so we can try doing half dispensing
            DoDelay = Dispensers[DispenserID].TimeMSML * (mlToDo > ShotDispenserML ? ShotDispenserML : mlToDo);
            mlToDo -= ShotDispenserML;
            Serial.println("Wait for dispening " + String(DoDelay) + "ms");

            MyDelay(DoDelay);

            Stepper_Z.moveTo(0);
            while (Stepper_Z.run()) yield();
            if (DoAmount - i > 0)                               //If another one is required
              MyDelay(Dispensers[DispenserID].TimeMSoff);
          }
        } break;
      case PUMP: {
          if (Dispensers[DispenserID].LocationZ <= Pump_Amount) {
            MoveTo(Dispensers[DispenserID].LocationX, Dispensers[DispenserID].LocationY);
            digitalWrite(PDO_Pump[Dispensers[DispenserID].LocationZ], HIGH);
            MyDelay(Dispensers[DispenserID].TimeMSML);
            digitalWrite(PDO_Pump[Dispensers[DispenserID].LocationZ], LOW);
            MyDelay(Dispensers[DispenserID].TimeMSoff);
          }
        }
        break;
      default:
        Serial.println("Error, unknown Dispenser type, ID=" + String(DispenserID) + " Type=" + String(Dispensers[DispenserID].Type));
        break;
    }
  } else { //No dispencer availble,
    MoveTo(Manual_X, Manual_Y);
    Serial.println("TODO, Wait for user confirmation, Drink is no in the machine so should be done manually, msg = '" + String(IN.Action) + "'");
    MyDelay(2500);
  }
}
