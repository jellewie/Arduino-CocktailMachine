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
#define PreFix_0_Ingredient "ai"
#define PreFix_0_Action     "aa"
#define PreFix_0_Ml         "am"
#define PreFix_1_Ingredient "bi"
#define PreFix_1_Action     "ba"
#define PreFix_1_Ml         "bm"
#define PreFix_2_Ingredient "ci"
#define PreFix_2_Action     "ca"
#define PreFix_2_Ml         "cm"
#define PreFix_3_Ingredient "di"
#define PreFix_3_Action     "da"
#define PreFix_3_Ml         "dm"
#define PreFix_4_Ingredient "ei"
#define PreFix_4_Action     "ea"
#define PreFix_4_Ml         "em"
#define PreFix_5_Ingredient "fi"
#define PreFix_5_Action     "fa"
#define PreFix_5_Ml         "fm"
#define PreFix_6_Ingredient "gi"
#define PreFix_6_Action     "ga"
#define PreFix_6_Ml         "gm"
#define PreFix_7_Ingredient "hi"
#define PreFix_7_Action     "ha"
#define PreFix_7_Ml         "hm"

void handle_Set() {
  Serial.print("SV: /SET?");
  String ERRORMSG;                                              //emthy=no error
  bool WriteEEPROM = false;
  bool makeMix = false;
  byte DoHoming = -1;
  Drink Mix;
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
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixMotorMAXAccel) {
      if (!WiFiManagerUser_Set_Value(2, ArgValue)) {
        ERRORMSG = "MotorMAXAccel Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixBedSize_X) {
      if (!WiFiManagerUser_Set_Value(3, ArgValue)) {
        ERRORMSG = "BedSize_X Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixBedSize_Y) {
      if (!WiFiManagerUser_Set_Value(4, ArgValue)) {
        ERRORMSG = "BedSize_Y Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixBedSize_Z) {
      if (!WiFiManagerUser_Set_Value(5, ArgValue)) {
        ERRORMSG = "BedSize_Z Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixManual_X) {
      if (!WiFiManagerUser_Set_Value(6, ArgValue)) {
        ERRORMSG = "Manual_X Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixManual_Y) {
      if (!WiFiManagerUser_Set_Value(7, ArgValue)) {
        ERRORMSG = "Manual_Y Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixShotDispenserML) {
      if (!WiFiManagerUser_Set_Value(8, ArgValue)) {
        ERRORMSG = "ShotDispenserML Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixHomeMAXSpeed) {
      if (!WiFiManagerUser_Set_Value(9, ArgValue)) {
        ERRORMSG = "HomeMAXSpeed Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixHomedistanceBounce) {
      if (!WiFiManagerUser_Set_Value(10, ArgValue)) {
        ERRORMSG = "HomedistanceBounce Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixDisableSteppersAfterMixDone) {
      if (!WiFiManagerUser_Set_Value(11, ArgValue)) {
        ERRORMSG = "DisableSteppersAfterMixDone Not set";
      } else {
        WriteEEPROM = true;
      }
    } else if (ArguName == PreFixHome) {
      DoHoming = true;
    } else if (ArguName == PreFix_Mix_Name) {
      Mix.Name = IngredientStringToID(ArgValue);
    } else if (ArguName == PreFix_0_Ingredient) {
      Mix.Ingredients[0].ID = IngredientStringToID(ArgValue); makeMix = true;
      makeMix = true;
    } else if (ArguName == PreFix_0_Action) {
      Mix.Ingredients[0].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_0_Ml) {
      Mix.Ingredients[0].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_1_Ingredient) {
      Mix.Ingredients[1].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_1_Action) {
      Mix.Ingredients[1].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_1_Ml) {
      Mix.Ingredients[1].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_2_Ingredient) {
      Mix.Ingredients[2].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_2_Action) {
      Mix.Ingredients[2].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_2_Ml) {
      Mix.Ingredients[2].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_3_Ingredient) {
      Mix.Ingredients[3].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_3_Action) {
      Mix.Ingredients[3].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_3_Ml) {
      Mix.Ingredients[3].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_4_Ingredient) {
      Mix.Ingredients[4].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_4_Action) {
      Mix.Ingredients[4].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_4_Ml) {
      Mix.Ingredients[4].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_5_Ingredient) {
      Mix.Ingredients[5].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_5_Action) {
      Mix.Ingredients[5].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_5_Ml) {
      Mix.Ingredients[5].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_6_Ingredient) {
      Mix.Ingredients[6].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_6_Action) {
      Mix.Ingredients[6].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_6_Ml) {
      Mix.Ingredients[6].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_7_Ingredient) {
      Mix.Ingredients[7].ID = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_7_Action) {
      Mix.Ingredients[7].Action = IngredientStringToID(ArgValue);
      makeMix = true;
    } else if (ArguName == PreFix_7_Ml) {
      Mix.Ingredients[7].ml = IngredientStringToID(ArgValue);
      makeMix = true;
    } else {
      ERRORMSG += "UNK argument" + ArguName + " = " + ArgValue + "/n";
    }
  }
  Serial.println(Mix.Name);

  Drink MixEmthy;
  if (makeMix and Mix.Name == "") {
    ERRORMSG += "No mix name given/n";
  } else if (!makeMix and Mix.Name != "") {
    ERRORMSG += "No mix ingredients given/n";
  }

  if (ERRORMSG == "") {
    server.send(200, "application/json", "Succes");
  } else {
    server.send(400, "application/json", ERRORMSG);
  }
  
  if (WriteEEPROM)
    WiFiManager.WriteEEPROM();
    
  if (DoHoming == 0) {
    DisableSteppers();
  } else if (DoHoming == 1) {
    Home();
  }
  
  if (Mix.Name != "") {
    Serial.println("MakeCocktail " + String(Mix.Name));
    MakeCocktail(Mix);
    if (DisableSteppersAfterMixDone)
      DisableSteppers();
  }
}
void handle_Get() {
  String Json = "{\"Drinks\": [";
  for (byte i = 0; i < Drinks_Amount; i++) {
    if (i != 0) Json += ",";
    Json += "{\"" + Drinks[i].Name + "\":[";
    if (IsAvailable(i))
      Json += "\"A\",";
    for (byte i = 0; i < 8; i++) {                                //For each Ingredient
      if (Drinks[i].Ingredients[i].ID != 0 or Drinks[i].Ingredients[i].Action != 0) {
        if (i != 0) Json += ",";
        Json += "[" + String(Drinks[i].Ingredients[i].ID) + ",\"" + Drinks[i].Ingredients[i].Action + "\"," + String(Drinks[i].Ingredients[i].ml) + "]";
      }
    }
    Json += "]}";
  }
  Json += "],\"Dispencers\":[";
  for (byte i = 0; i < Dispensers_Amount; i++) {
    if (Dispensers[i].Type != UNSPECIFIED) {
      if (i != 0) Json += ",";
      Json += "[" + String(Dispensers[i].Type) + "," + String(Dispensers[i].LocationX) + "," + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].LocationZ) + "," + String(Dispensers[i].TimeMSML) + "," + String(Dispensers[i].TimeMSoff) + "," + String(Dispensers[i].IngredientID) + "]";
    }
  }
  Json += "],\"Ingredients\":[";
  for (byte i = 0; i < Ingredient_Amount; i++) {
    if (i != 0) Json += ",";
    Json += "\"" + IngredientIDtoString(i) + "\"";
  }
  Json += "],\"Settings\":[";
  Json += "\"Homed\":" + IsTrueToString(Homed) + ",";
  Json += "]";

  Json += "}";
  server.send(200, "text/html", Json);
}
void handle_Info() {
  String Message = "https://github.com/jellewie \n"
                   "Code compiled on " + String(__DATE__) + " " + String(__TIME__) + "\n"
                   "MAC adress = " + String(WiFi.macAddress()) + "\n"
                   "IP adress = " + IpAddress2String(WiFi.localIP()) + "\n"

                   "\nSOFT_SETTINGS\n";
  for (byte i = 3; i < WiFiManager_Settings + 1; i++)
    Message += WiFiManager_VariableNames[i - 1] + " = " + WiFiManager.Get_Value(i, false, true) + "\n";
  server.send(200, "text/plain", Message);
}
void handle_OnConnect() {
  String HTML = "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, viewport-fit=cover\"><strong>Cool mixer thingy</strong>";

  HTML += "<br><br><strong>Available mixes:</strong><br><br><form method=\"POST\" action=\"set\"><label for=\"mix\">Choose a mix:</label><select id=\"mix\" name=\"mix\">";
  for (byte i = 0; i < Drinks_Amount; i++) {
    if (IsAvailable(i))
      HTML += "<option value=\"" + String(i) + "\">" + Drinks[i].Name + "</option>";
  }
  HTML += "</select><input type=\"submit\" value=\"Submit\"></form>";

  HTML += "<br><br><strong>All mixes:</strong><form action=\"/set\"><label for=\"mix\">Choose a mix:</label><select id=\"mix\" name=\"mix\">";
  for (byte i = 0; i < Drinks_Amount; i++)
    HTML += "<option value=\"" + String(i) + "\">" + Drinks[i].Name + "</option>";
  HTML += "</select><input type=\"submit\" value=\"Submit\"></form>";

  HTML += "<br><br><form action=\"/info\"><button>Info</button></form>";
  HTML += "<form action=\"/ota\"><button>OTA page</button></form>";
  HTML += "<form action=\"/ip\"><button>Softsettings</button></form>";
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
