//===========================================================================
//"/set" commands
//===========================================================================
//Settings
#define PreFixMotorMAXSpeed "ms"
#define PreFixMotorMAXAccel "ma"
#define PreFixBedSize_X "sx"
#define PreFixBedSize_Y "sy"
#define PreFixBedSize_Z "sz"
#define PreFixManual_X "mx"
#define PreFixManual_Y "my"
#define PreFixShotDispenserML "dv"
#define PreFixHomeMAXSpeed "hs"
#define PreFixHomedistanceBounce "hd"
#define PreFixDisableSteppersAfterMixDone "ds"
#define PreFixHome "h"
//Dispenser Settings
#define PreFixSetDispenserType "dt"
#define PreFixSetDispenserX "dx"
#define PreFixSetDispenserY "dy"
#define PreFixSetDispenserZ "dz"
#define PreFixSetDispenserMSml "dl"
#define PreFixSetDispenserMSoff "do"
#define PreFixSetDispenserIngredientID "dn"
//Create a mix (max 8 ingredient)
#define PreFix_Mix_Name     "m"            //this one MUST be set (=!"") to start creating a mix 
#define PreFix_0_Ingredient "i0"
#define PreFix_0_Action     "a0"
#define PreFix_0_Ml         "m0"
#define PreFix_1_Ingredient "i1"
#define PreFix_1_Action     "a1"
#define PreFix_1_Ml         "m1"
#define PreFix_2_Ingredient "i2"
#define PreFix_2_Action     "a2"
#define PreFix_2_Ml         "m2"
#define PreFix_3_Ingredient "i3"
#define PreFix_3_Action     "a3"
#define PreFix_3_Ml         "m3"
#define PreFix_4_Ingredient "i4"
#define PreFix_4_Action     "a4"
#define PreFix_4_Ml         "m4"
#define PreFix_5_Ingredient "i5"
#define PreFix_5_Action     "a5"
#define PreFix_5_Ml         "m5"
#define PreFix_6_Ingredient "i6"
#define PreFix_6_Action     "a6"
#define PreFix_6_Ml         "m6"
#define PreFix_7_Ingredient "i7"
#define PreFix_7_Action     "a7"
#define PreFix_7_Ml         "m7"
//Debug, move to XYZ
#define PreFix_X         "x"
#define PreFix_Y         "y"
#define PreFix_Z         "z"

#include "client.h"

void handle_Set() {
  Serial.print("SV: /SET?");
  String ERRORMSG;                                              //emthy=no error
  bool makeMix = false;
  byte DoHoming = -1;
  int GoTo[3];
  Drink Mix;
  for (byte i = 0; i < 8; i++) {
    Mix.Ingredients[i].ID = 0;
    Mix.Ingredients[i].Action = "";
    Mix.Ingredients[i].ml = 0;
  }
  for (int i = 0; i < server.args(); i++) {
    String ArguName = server.argName(i);
    ArguName.toLowerCase();
    String ArgValue = server.arg(i);
    ArgValue.toLowerCase();
    Serial.print(" " + ArguName + "=" + ArgValue);
    if (ArguName == PreFixMotorMAXSpeed) {
      if (!WiFiManagerUser_Set_Value(1, ArgValue)) {
        ERRORMSG = "MotorMAXSpeed Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixMotorMAXAccel) {
      if (!WiFiManagerUser_Set_Value(2, ArgValue)) {
        ERRORMSG = "MotorMAXAccel Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixBedSize_X) {
      if (!WiFiManagerUser_Set_Value(3, ArgValue)) {
        ERRORMSG = "BedSize_X Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixBedSize_Y) {
      if (!WiFiManagerUser_Set_Value(4, ArgValue)) {
        ERRORMSG = "BedSize_Y Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixBedSize_Z) {
      if (!WiFiManagerUser_Set_Value(5, ArgValue)) {
        ERRORMSG = "BedSize_Z Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixManual_X) {
      if (!WiFiManagerUser_Set_Value(6, ArgValue)) {
        ERRORMSG = "Manual_X Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixManual_Y) {
      if (!WiFiManagerUser_Set_Value(7, ArgValue)) {
        ERRORMSG = "Manual_Y Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixShotDispenserML) {
      if (!WiFiManagerUser_Set_Value(8, ArgValue)) {
        ERRORMSG = "ShotDispenserML Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixHomeMAXSpeed) {
      if (!WiFiManagerUser_Set_Value(9, ArgValue)) {
        ERRORMSG = "HomeMAXSpeed Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixHomedistanceBounce) {
      if (!WiFiManagerUser_Set_Value(10, ArgValue)) {
        ERRORMSG = "HomedistanceBounce Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixDisableSteppersAfterMixDone) {
      if (!WiFiManagerUser_Set_Value(11, ArgValue)) {
        ERRORMSG = "DisableSteppersAfterMixDone Not set";
      } else {
        SaveEEPROMinSeconds = 30;
      }
    } else if (ArguName == PreFixHome) {
      DoHoming = true;
    } else if (ArguName == PreFix_Mix_Name) {
      Mix.Name = ArgValue;
    } else if (ArguName == PreFix_0_Ingredient) {
      Mix.Ingredients[0].ID = IngredientStringToID(ArgValue); makeMix = true;
      makeMix = true;
    } else if (ArguName == PreFix_0_Action) {
      Mix.Ingredients[0].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_0_Ml) {
      Mix.Ingredients[0].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_1_Ingredient) {
      Mix.Ingredients[1].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_1_Action) {
      Mix.Ingredients[1].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_1_Ml) {
      Mix.Ingredients[1].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_2_Ingredient) {
      Mix.Ingredients[2].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_2_Action) {
      Mix.Ingredients[2].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_2_Ml) {
      Mix.Ingredients[2].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_3_Ingredient) {
      Mix.Ingredients[3].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_3_Action) {
      Mix.Ingredients[3].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_3_Ml) {
      Mix.Ingredients[3].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_4_Ingredient) {
      Mix.Ingredients[4].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_4_Action) {
      Mix.Ingredients[4].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_4_Ml) {
      Mix.Ingredients[4].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_5_Ingredient) {
      Mix.Ingredients[5].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_5_Action) {
      Mix.Ingredients[5].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_5_Ml) {
      Mix.Ingredients[5].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_6_Ingredient) {
      Mix.Ingredients[6].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_6_Action) {
      Mix.Ingredients[6].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_6_Ml) {
      Mix.Ingredients[6].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_7_Ingredient) {
      Mix.Ingredients[7].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_7_Action) {
      Mix.Ingredients[7].Action = ArgValue;
      makeMix = true;
    } else if (ArguName == PreFix_7_Ml) {
      Mix.Ingredients[7].ml = ArgValue.toInt();
      makeMix = true;
    } else if (ArguName == PreFix_X) {
      GoTo[0] = ArgValue.toInt();
    } else if (ArguName == PreFix_Y) {
      GoTo[1] = ArgValue.toInt();
    } else if (ArguName == PreFix_Z) {
      GoTo[2] = ArgValue.toInt();
    } else {
      ERRORMSG += "UNK argument" + ArguName + " = " + ArgValue + "/n";
    }
  }
  Serial.println();

  Drink MixEmthy;
  if (makeMix and Mix.Name == "") {
    ERRORMSG += "No mix name given/n";
  } else if (!makeMix and Mix.Name != "") {
    ERRORMSG += "No mix ingredients given/n";
  }

  if (ERRORMSG == "") {
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "application/json", ERRORMSG);
  }

  if (DoHoming == 0) {
    DisableSteppers();
  } else if (DoHoming == 1) {
    Home();
  }

  if (GoTo[0] != 0 or GoTo[1]!= 0 or GoTo[3]!= 0){
    MoveTo(GoTo[0], GoTo[1],GoTo[2]);
  }
  
  if (Mix.Name != "") {
    Serial.println("MakeCocktail " + String(Mix.Name));
    MakeCocktail(Mix);
    if (DisableSteppersAfterMixDone)
      DisableSteppers();
  }
}
void handle_Get() {
  String Json = "{\"dispensers\":[";
  for (byte i = 0; i < Dispensers_Amount; i++) {
    if (i != 0) Json += ",";
    Json += "[" + String(Dispensers[i].Type) + "," + String(Dispensers[i].LocationX) + "," + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].LocationZ) + "," + String(Dispensers[i].TimeMSML) + "," + String(Dispensers[i].TimeMSoff) + "," + String(Dispensers[i].IngredientID) + "]";
  }
  Json += "],\"ingredients\":[";
  for (byte i = 0; i < Ingredient_Amount; i++) {
    if (i != 0) Json += ",";
    Json += "\"" + IngredientIDtoString(i) + "\"";
  }
  Json += "],\"settings\":{";
  Json += "\"homed\":" + IsTrueToString(Homed);
  Json += ",\"disableSteppersAfterMixDone\":" + IsTrueToString(DisableSteppersAfterMixDone);
  Json += ",\"bedSizeX\":" + String(BedSize_X);
  Json += ",\"bedSizeY\":" + String(BedSize_Y);
  Json += ",\"bedSizeZ\":" + String(BedSize_Z);
  Json += ",\"manualX\":" + String(Manual_X);
  Json += ",\"manualY\":" + String(Manual_Y);
  Json += ",\"motorMaxSpeed\":" + String(MotorMAXSpeed);
  Json += ",\"motorMaxAccel\":" + String(MotorMAXAccel);
  Json += ",\"shotDispenserMl\":" + String(ShotDispenserML);
  Json += ",\"homeMaxSpeed\":" + String(HomeMAXSpeed);
  Json += ",\"homedistanceBounce\":" + String(HomedistanceBounce);
  Json += ",\"naxGlassSize\":" + String(MaxGlassSize);
  Json += "}";

  Json += "}";
  server.send(200, "text/html", Json);
}
void handle_Info() {
  String Message = "https://github.com/jellewie \n"
                   "Code compiled on " + String(__DATE__) + " " + String(__TIME__) + "\n"
                   "MAC adress = " + String(WiFi.macAddress()) + "\n"
                   "IP adress = " + IpAddress2String(WiFi.localIP()) + "\n"
                   "X_Ref = " + IsTrueToString(digitalRead(PDI_X_Ref) == LOW) + "\n"
                   "Y_Ref = " + IsTrueToString(digitalRead(PDI_Y_Ref) == LOW) + "\n"
                   "Z_Ref = " + IsTrueToString(digitalRead(PDI_Z_Ref) == LOW) + "\n"
                   "Switch = " + IsTrueToString(digitalRead(PDI_S) == LOW) + "\n"

                   "\nSOFT_SETTINGS\n";
  for (byte i = 3; i < WiFiManager_Settings + 1; i++)
    Message += WiFiManager_VariableNames[i - 1] + " = " + WiFiManager.Get_Value(i, false, true) + "\n";
  server.send(200, "text/plain", Message);
}
void handle_OnConnect() {
  server.send(200, "text/html", HTML);
}
void handle_Reset() {
  if (WiFiManager.ClearEEPROM()) {
    server.send(200, "text/plain", "EEPROM cleared");
    MyDelay(10);
    ESP.restart();                                              //Restart the ESP
  }
  server.send(400, "text/plain", "Error trying to clear EEPROM");
}
void handle_NotFound() {
  String Message = "ERROR URL NOT FOUND: '";
  Message += (server.method() == HTTP_GET) ? "GET" : "POST";
  Message += server.uri();
  if (server.args() > 0) Message += " ? ";
  for (byte i = 0; i < server.args(); i++) {
    if (i != 0)
      Message += "&";
    Message += server.argName(i) + " = " + server.arg(i);
  }
  server.send(404, "text / plain", Message);
#ifdef Server_SerialEnabled
  Serial.println("SV : 404 " + Message);
#endif //Server_SerialEnabled
}
