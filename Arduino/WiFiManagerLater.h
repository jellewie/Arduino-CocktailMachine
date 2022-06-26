/* Written by JelleWho https://github.com/jellewie
   https://github.com/jellewie/Arduino-WiFiManager
*/
//===========================================================================
// Things that can/need to be defined after including "WiFiManager.h"
//===========================================================================
const byte Pin_LED  = LED_BUILTIN;                              //Just here for some examples, It's the LED to give feedback on (like blink on error)
bool WiFiManagerUser_Set_Value(byte ValueID, String Value) {
  switch (ValueID) {                                            //Note the numbers are shifted from what is in memory, 0 is the first user value
    case 0: {
        if (Value.length() > sizeof(Name))        return false;	//Length is to long, it would not fit so stop here
        Value.toCharArray(Name, 16);              return true;
      } break;
    case 1: {
        if (!StringIsDigit(Value))                return false;
        MotorMAXSpeed = Value.toInt();            return true;
      } break;
    case 2: {
        if (!StringIsDigit(Value))                return false;
        MotorMAXAccel = Value.toInt();            return true;
      } break;
    case 3: {
        if (!StringIsDigit(Value))                return false;
        BedSize_X = Value.toInt();                return true;
      } break;
    case 4: {
        if (!StringIsDigit(Value))                return false;
        BedSize_Y = Value.toInt();                return true;
      } break;
    case 5: {
        if (!StringIsDigit(Value))                return false;
        BedSize_Z = Value.toInt();                return true;
      } break;
    case 6: {
        if (!StringIsDigit(Value))                return false;
        Manual_X = Value.toInt();                 return true;
      } break;
    case 7: {
        if (!StringIsDigit(Value))                return false;
        Manual_Y = Value.toInt();                 return true;
      } break;
    case 8: {
        if (!StringIsDigit(Value))                return false;
        int Temp = Value.toInt();
        if (Temp < 0 or Temp > 255)               return false;
        ShotDispenserML = Temp;                   return true;
      } break;
    case 9: {
        if (!StringIsDigit(Value))                return false;
        HomeMAXSpeed = Value.toInt();             return true;
      } break;
    case 10: {
        if (!StringIsDigit(Value))                return false;
        HomedistanceBounce = Value.toInt();       return true;
      } break;
    case 11: {
        DisableSteppersAfterMixDone = IsTrue(Value); return true;
      } break;
    default: {
        byte i = ValueID - 12;                                  //Remove the amount above from the counter, so the next are of the Dispensers
        if (i <= Dispensers_Amount) {
          String _Output[7];
          CutVariable(Value, &_Output[0], 7);
          Dispenser Dis;
          Dis.Type = _Output[0].toInt();
          Dis.LocationX = _Output[1].toInt();
          Dis.LocationY = _Output[2].toInt();
          Dis.LocationZ = _Output[3].toInt();
          Dis.TimeMSML = _Output[4].toInt();
          Dis.TimeMSoff = _Output[5].toInt();
          Dis.IngredientID = IngredientStringToID(_Output[6]);
          SetDispencer(Dis, i);
          return true;
        }
      }
  }
  return false;                                                 //Report back that the ValueID is unknown, and we could not set it
}
String WiFiManagerUser_Get_Value(byte ValueID, bool Safe, bool Convert) {
  //if its 'Safe' to return the real value (for example the password will return '****' or '1234')
  //'Convert' the value to a readable string for the user (bool '0/1' to 'FALSE/TRUE')
  switch (ValueID) {                                            //Note the numbers are shifted from what is in memory, 0 is the first user value
    case 0:  return String(Name);                                  break;
    case 1:  return String(MotorMAXSpeed);                         break;
    case 2:  return String(MotorMAXAccel);                         break;
    case 3:  return String(BedSize_X);                             break;
    case 4:  return String(BedSize_Y);                             break;
    case 5:  return String(BedSize_Z);                             break;
    case 6:  return String(Manual_X);                              break;
    case 7:  return String(Manual_Y);                              break;
    case 8:  return String(ShotDispenserML);                       break;
    case 9:  return String(HomeMAXSpeed);                          break;
    case 10: return String(HomedistanceBounce);                    break;
    case 11: return IsTrueToString(DisableSteppersAfterMixDone);   break;
    default: {
        byte i = ValueID - 12;                                  //Remove the amount above from the counter, so the next are of the Dispensers
        if (i <= Dispensers_Amount) {
          String Output = String(Dispensers[i].Type) + "," + String(Dispensers[i].LocationX) + "," + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].LocationZ) + "," + String(Dispensers[i].TimeMSML) + "," + String(Dispensers[i].TimeMSoff) + "," + String(Dispensers[i].IngredientID);
          return String(Output);
        }
      }
  }
  return "";
}
void WiFiManagerUser_Status_Start() {                           //Called before start of WiFi
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, HIGH);
}
void WiFiManagerUser_Status_Done() {                            //Called after succesfull connection to WiFi
  digitalWrite(Pin_LED, LOW);
}
void WiFiManagerUser_Status_Blink() {                           //Used when trying to connect/not connected
  digitalWrite(Pin_LED, !digitalRead(Pin_LED));
}
void WiFiManagerUser_Status_StartAP() {}                        //Called before start of APmode
bool WiFiManagerUser_HandleAP() {                               //Called when in the While loop in APMode, this so you can exit it
  //Return true to leave APmode
#define TimeOutApMode 15 * 60 * 1000;                           //Example for a timeout, (time in ms)
  unsigned long StopApAt = millis() + TimeOutApMode;
  if (millis() > StopApAt)    return true;                      //If we are running for to long, then flag we need to exit APMode
  return false;
}
