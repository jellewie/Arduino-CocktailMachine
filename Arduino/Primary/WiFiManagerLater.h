/* Written by JelleWho https://github.com/jellewie
   https://github.com/jellewie/Arduino-WiFiManager
*/
//===========================================================================
// Things that can/need to be defined after including "WiFiManager.h"
//===========================================================================
const uint8_t Pin_LED = LED_BUILTIN;  //Just here for some examples, It's the LED to give feedback on (like blink on error)
bool ApStarted = false;
bool WiFiManagerUser_Set_Value(uint8_t ValueID, String Value) {
  switch (ValueID) {  //Note the numbers are shifted from what is in memory, 0 is the first user value
    case 0:
      {
        if (Value.length() > sizeof(Name)) return false;  //Length is to long, it would not fit so stop here
        Value.toCharArray(Name, 16);
        return true;
      }
      break;
    case 1:
      {
        if (!StringIsDigit(Value)) return false;
        MotorMAXSpeed = Value.toInt();
        return true;
      }
      break;
    case 2:
      {
        if (!StringIsDigit(Value)) return false;
        MotorMAXAccel = Value.toInt();
        return true;
      }
      break;
    case 3:
      {
        if (!StringIsDigit(Value)) return false;
        BedSize_X = Value.toInt();
        return true;
      }
      break;
    case 4:
      {
        if (!StringIsDigit(Value)) return false;
        BedSize_Y = Value.toInt();
        return true;
      }
      break;
    case 5:
      {
        return false;
      }
      break;
    case 6:
      {
        if (!StringIsDigit(Value)) return false;
        Manual_X = Value.toInt();
        return true;
      }
      break;
    case 7:
      {
        if (!StringIsDigit(Value)) return false;
        Manual_Y = Value.toInt();
        return true;
      }
      break;
    case 8:
      {
        return false;
      }
      break;
    case 9:
      {
        if (!StringIsDigit(Value)) return false;
        HomeMAXSpeed = Value.toInt();
        return true;
      }
      break;
    case 10:
      {
        if (!StringIsDigit(Value)) return false;
        MaxHomeBounce = Value.toInt();
        return true;
      }
      break;
    case 11:
      {
        DisableSteppersAfterIdleS = Value.toInt();
        return true;
      }
      break;
    case 12:
      {
        if (!StringIsDigit(Value)) return false;
        MaxGlassSize = Value.toInt();
        return true;
      }
      break;
    case 13:
      {
        if (!StringIsDigit(Value)) return false;
        uint16_t Temp = Value.toInt();
        if (Temp < 0 or Temp > 255) return false;
        MaxBrightness = Temp;
        return true;
      }
      break;
    case 14:
      {
        //reserved
      }
      break;
    case 15:
      {
        //reserved
      }
      break;
    default:
      {
        uint8_t i = ValueID - 16;  //Remove the amount above from the counter, so the next ones are of the Dispensers
        if (i < Dispensers_Amount) {
          String _Output[5];
          CutVariable(Value, &_Output[0], 5);
          _Output[0].replace("X=", "");
          _Output[1].replace("Y=", "");
          _Output[2].replace("MSML", "");
          _Output[3].replace("MSoff", "");
          if (!StringIsDigit(_Output[0]) or !StringIsDigit(_Output[1]) or !StringIsDigit(_Output[2]) or !StringIsDigit(_Output[3])) return false;
          Dispenser Dis;
          Dis.LocationX = _Output[0].toInt();
          Dis.LocationY = _Output[1].toInt();
          Dis.TimeMSML = _Output[2].toInt();
          Dis.DelayAir = _Output[3].toInt();
          Dis.IngredientID = 0;  //We do not do _Output[4] since thats stored in the dispenser
          SetDispenser(Dis, i);
          return true;
        }
      }
  }
  return false;  //Report back that the ValueID is unknown, and we could not set it
}
String WiFiManagerUser_Get_Value(uint8_t ValueID, bool Safe, bool Convert) {
  //if its 'Safe' to return the real value (for example the password will return '****' or '1234')
  //'Convert' the value to a readable string for the user (bool '0/1' to 'FALSE/TRUE')
  switch (ValueID) {  //Note the numbers are shifted from what is in memory, 0 is the first user value
    case 0: return String(Name); break;
    case 1: return String(MotorMAXSpeed); break;
    case 2: return String(MotorMAXAccel); break;
    case 3: return String(BedSize_X); break;
    case 4: return String(BedSize_Y); break;
    case 5: return ""; break;
    case 6: return String(Manual_X); break;
    case 7: return String(Manual_Y); break;
    case 8: return ""; break;
    case 9: return String(HomeMAXSpeed); break;
    case 10: return String(MaxHomeBounce); break;
    case 11: return String(DisableSteppersAfterIdleS); break;
    case 12: return String(MaxGlassSize); break;
    case 13: return String(MaxBrightness); break;
    case 14: return ""; break;
    case 15: return ""; break;
    default:
      {
        uint8_t i = ValueID - 16;  //Remove the amount above from the counter, so the next are of the Dispensers
        if (i < Dispensers_Amount) {
          String Output = "";
          if (Convert) {
            Output = String(i) + ",X=" + String(Dispensers[i].LocationX) + ",Y=" + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].TimeMSML) + "MSML," + String(Dispensers[i].DelayAir) + "MSoff," + IngredientIDtoString(Dispensers[i].IngredientID);
          } else {
            Output = String(i) + "," + String(Dispensers[i].LocationX) + "," + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].TimeMSML) + "," + String(Dispensers[i].DelayAir) + "," + String(Dispensers[i].IngredientID);
          }
          return String(Output);
        }
      }
  }
  return "";
}
void WiFiManagerUser_Status_Start() {  //Called before start of WiFi
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, HIGH);
  LcdPrint("", "WiFi connecting");
  ApStarted = false;
}
void WiFiManagerUser_Status_Done() {  //Called after succesfull connection to WiFi
  digitalWrite(Pin_LED, LOW);
  LcdPrint("", "WiFi connected");
  ApStarted = false;
}
void WiFiManagerUser_Status_Blink() {  //Used when trying to connect/not connected
  digitalWrite(Pin_LED, !digitalRead(Pin_LED));
}
bool WiFiManagerUser_HandleAP() {  //Called when in the While loop in APMode, this so you can exit it
  if (ApStarted == false) {
    ApStarted = true;
    LcdPrint("", "ApMode");
  }
  //Return true to leave APmode
#define TimeOutApMode 15 * 60 * 1000;  //Example for a timeout, (time in ms)
  unsigned long StopApAt = millis() + TimeOutApMode;
  if (millis() > StopApAt) return true;  //If we are running for to long, then flag we need to exit APMode
  return false;
}
