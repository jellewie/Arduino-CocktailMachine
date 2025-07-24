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
        if (!StringIsDigit(Value)) return false;
        uint16_t Temp = Value.toInt();
        if (Temp < 30 or Temp > 255) return false;  //Dont allow faster speeds than 30s, it takes up to 4 seconds to check if a dispenser went offline
        DispenserHeartbeat = Temp;
        return true;
      }
      break;
    case 15:
      {
        if (!StringIsDigit(Value)) return false;
        uint16_t Temp = Value.toInt();
        DispenserDripTime = Temp;
        return true;
      }
      break;
    case 16:
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
          _Output[3].replace("MSdelayed", "");
          //_Output[4].replace("", ""); //IngredientID has no pre of suffix
          _Output[5].replace("MLFluidLeft", "");
          if (!StringIsDigit(_Output[0]) or !StringIsDigit(_Output[1]) or !StringIsDigit(_Output[2]) or !StringIsDigit(_Output[3])) return false;
          Dispenser Dis;
          Dis.LocationX = _Output[0].toInt();
          Dis.LocationY = _Output[1].toInt();
          Dis.TimeMSML = 0;      //We do not do this one since thats stored in the dispenser
          Dis.DelayAir = 0;      //^
          Dis.IngredientID = 0;  //^
          Dis.FluidLevel = 0;    //^
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
    case 14: return String(DispenserHeartbeat); break;
    case 15: return String(DispenserDripTime); break;
    case 16: return ""; break;
    default:
      {
        uint8_t i = ValueID - 16;  //Remove the amount above from the counter, so the next are of the Dispensers
        if (i < Dispensers_Amount) {
          String Output = "";
          if (Convert) {
            Output = "X=" + String(Dispensers[i].LocationX) + ",Y=" + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].TimeMSML) + "MSML," + String(Dispensers[i].DelayAir) + "MSdelayed," + IngredientIDtoString(Dispensers[i].IngredientID) + "," + String(Dispensers[i].FluidLevel) + "MLFluidLeft";
          } else {
            Output = String(Dispensers[i].LocationX) + "," + String(Dispensers[i].LocationY) + "," + String(Dispensers[i].TimeMSML) + "," + String(Dispensers[i].DelayAir) + "," + String(Dispensers[i].IngredientID) + "," + String(Dispensers[i].FluidLevel);
          }
          return String(Output);
        }
      }
  }
  return "";
}
void WiFiManagerUser_Status_Start() {                       //Called before start of WiFi
  LED_Fill(0, TotalLEDs, CRGB(255, 0, 255));                //Turn all LEDs purple 2222
  LED_Fill(0, TotalLEDs / 4, CRGB(0, 0, 255));              //Turn 1th quater blue 1222
  LED_Fill(TotalLEDs / 2, TotalLEDs / 4, CRGB(0, 0, 255));  //Turn 2rd quater blue 1212
  FastLED.show();                                           //Update leds to show wifi is starting
  pinMode(Pin_LED, OUTPUT);
  digitalWrite(Pin_LED, HIGH);
  LcdPrint("", "WiFi connecting");
  ApStarted = false;
}
void WiFiManagerUser_Status_Done() {        //Called after succesfull connection to WiFi
  LED_Fill(0, TotalLEDs, CRGB(0, 255, 0));  //Turn all Clock LEDs green
  FastLED.show();                           //Update leds to show wifi is done
  digitalWrite(Pin_LED, LOW);
  LcdPrint("", "WiFi connected");
  ApStarted = false;
}
void WiFiManagerUser_Status_Blink() {  //Used when trying to connect/not connected
  digitalWrite(Pin_LED, !digitalRead(Pin_LED));
}
void WiFiManagerUser_Status_StartAP() {                     //Called before start of APmode
  LED_Fill(0, TotalLEDs, CRGB(255, 0, 255));                //Turn all LEDs purple 2222
  LED_Fill(0, TotalLEDs / 4, CRGB(255, 0, 0));              //Turn 1th quater red  1222
  LED_Fill(TotalLEDs / 2, TotalLEDs / 4, CRGB(255, 0, 0));  //Turn 2rd quater red  1212
  FastLED.show();                                           //Update leds to show we are entering APmode
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
