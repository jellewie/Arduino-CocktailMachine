/* Written by JelleWho https://github.com/jellewie
   https://github.com/jellewie/Arduino-WiFiManager

  These are some examples how to 'hook' functions with code into the WiFiManager.
  This file and all functions are not required, but when defined before '#include "WiFiManagerUser.h"' they will be hooked in to WiFiManager
  if you want to declair functions after including WiFiManager, uses a 'extern <function name>' to declair a dummy here, so the hook will be made, but will be hooked to a later (extern if I may say) declaration

  NOTES
   DO NOT USE char(") in any of input stings on the webpage, use char(') if you need it. char(") will be replaced
   These are the declaired triggered and function names: you can declare over these to overwrite them but be carefull
      server.on("/",        WiFiManager_handle_Connect);
      server.on("/ip",      WiFiManager_handle_Connect);     //Just as backup, so the "/" can be overwritten by user
      server.on("/setup",   WiFiManager_handle_Settings);
      server.on("/ota",     WiFiManager_OTA_handle_uploadPage);
      server.on("/update",  HTTP_POST, WiFiManager_OTA_handle_update, WiFiManager_OTA_handle_update2);

  HOW TO ADD CUSTOM VALUES
   -"WiFiManagerUser_VariableNames_Defined" define this, and ass custom names for the values
   -"WiFiManager_VariableNames"  Add the description name of the value to get/set to this list
   -"EEPROM_size"     [optional] Make sure it is big enough for your needs, SIZE_SSID+SIZE_PASS+YourValues (1 uint8_t = 1 character)
   -"Set_Value"       Set the action on what to do on startup with this value
   -"Get_Value"       [optional] Set the action on what to fill in in the boxes in the 'AP settings portal'
*/
//===========================================================================
// Things that need to be defined before including "WiFiManager.h"
//===========================================================================
//#define WiFiManager_SerialEnabled                               //Define to send Serial debug feedback

#define WiFiManagerUser_Set_Value_Defined     //Define we want to hook into WiFiManager
#define WiFiManagerUser_Get_Value_Defined     //^
#define WiFiManagerUser_Status_Start_Defined  //^
#define WiFiManagerUser_Status_Done_Defined   //^
#define WiFiManagerUser_Status_Blink_Defined  //^
//#define WiFiManagerUser_Status_StartAP_Defined                  //^
#define WiFiManagerUser_HandleAP_Defined  //^

#define WiFiManager_DoRequest  //Adds a simple way to do stable URL request (with optional json)
#define WiFiManager_Restart    //Adds a simple handle "/restart" to restart the ESP

#define WiFiManagerUser_VariableNames_Defined  //Define that we want to use the custom user variables (Dont forget to setup WiFiManager_VariableNames and WiFiManager_Settings)
const String WiFiManager_VariableNames[] = { "SSID", "Password", "name", "MotorMAXSpeed", "MotorMAXAccel", "BedSize_X", "BedSize_Y", "", "Manual_X", "Manual_Y", "", "HomeMAXSpeed", "MaxHomeBounce", "DisableSteppersAfterIdleS", "MaxGlassSize", "MaxBrightness", "Reserved", "Reserved", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "D10", "D11", "D12", "D13", "D14", "D15", "D16", "D17", "D18", "D19" };
const uint16_t EEPROM_size = 4096;  //Max Amount of chars for 'SSID(16) + PASSWORD(16) + extra custom vars(?) +1(NULL)' defaults to 3

#define WiFiManagerUser_Name_Defined
char Name[16] = "mixer";  //If you want to define the name somewhere else use 'char* Name = Name'

#define WiFiManager_mDNS  //Set up mDNS, this makes it so it responce to the url 'http://name.local/'

#define WiFiManager_OTA  //Define if you want to use the Over The Air update page (/ota)
#define WiFiManagerUser_UpdateWebpage_Defined
const String UpdateWebpage = "https://github.com/jellewie/Arduino-CocktailMachine/releases";  //Set an custom OTA update URL to show the user
//  WiFiManager.RunServer();                                    //(runtime) Note when using OTA, you need to run the server in your loop
//  WiFiManager.OTA_Enabled = false;                            //(runtime) Turn off/on OTA

//  WiFiManager.EnableSetup(true);                              //(runtime) Enable the settings, only enabled in APmode by default
//  WiFiManager.WriteEEPROM();                                  //(runtime) If you want to manually save the settings(EEPROM LIMITED WRITES! do not spam)

//#define WiFiManager_DNS                                       //Was causing some troubles with stableness, note sure why yet
