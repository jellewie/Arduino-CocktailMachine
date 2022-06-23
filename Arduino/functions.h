struct Ingredient {
  byte ID;
  String Action;                                               //If given, prompt the message and wait for user confirmation first
  byte ml;
};
struct Drink {
  String Name;
  Ingredient Ingredients[8];
};
enum TypeE {UNSPECIFIED, SHOTDispenser, PUMP};
struct Dispenser {
  byte Type;
  int LocationX;
  int LocationY;
  int LocationZ;         //Used for Pump_ID for PUMP
  int TimeMSML;          //Timer per ms for each mL
  int TimeMSoff;         //Delay in MS to wait after
  byte IngredientID;
};

extern Drink Drinks[];
extern byte Drinks_Amount;
extern String IngredientS[];
extern byte Ingredient_Amount;
extern Dispenser Dispensers[];

void CutVariable(String _Input, String *_Variable, byte _VariableLength) {
  //Takes in a string, and cuts them into parts; "test,yes,clock" => {"test","yes","clock"}
  //Returns the output in the same string, for example
  //  String _Output[6], _Input = "boop,boobs,good,no,wait,what";
  //  CutVariable(_Input, &_Output[0], 6);
  //  Serial.println(String(_Output[0]) + "_" + String(_Output[1]) + "_" + String(_Output[2]));
  byte _StartAt = 0, _WriteTo = 0;
  for (byte i = 0; i <= _Input.length(); i++) {                 //For each character in the input string
    if (_Input.charAt(i) == ',') {
      _Variable[_WriteTo] = _Input.substring(_StartAt, i);
      _WriteTo ++;
      _StartAt = i + 1;
      if (_WriteTo >= _VariableLength - 1) break;               //If last one
    }
  }
  _Variable[_WriteTo] = _Input.substring(_StartAt);
}
void SetDispencer(Dispenser Dis, byte i) {
  if (i < Dispensers_Amount)
    Dispensers[i] = Dis;
}
bool AddDispencer(Dispenser Dis) {
  for (byte i = 0; i < Dispensers_Amount; i++) {
    if (Dispensers[i].Type != UNSPECIFIED) {
      Dispensers[i] = Dis;
      return true;
    }
  }
  return false;
}
bool RemoveDispencer(byte IngredientID) {
  for (byte i = 0; i < Dispensers_Amount; i++) {
    if (Dispensers[i].IngredientID == IngredientID) {
      if (i != 0) {                                          //If this is the first one (needs to be ocupied)
        Dispensers[i].Type = UNSPECIFIED;
      } else {
        for (byte j = 1; j < Dispensers_Amount; j++) {
          if (Dispensers[j].Type != UNSPECIFIED) {
            Dispensers[i] = Dispensers[j];
            Dispensers[j].Type = UNSPECIFIED;
          }
        }
      }
      return true;
    }
  }
  return false;
}
bool TickEveryXms(unsigned long *_LastTime, unsigned long _Delay) {
  //With overflow, can be adjusted, no overshoot correction, true when (Now < _LastTime + _Delay)
  /* Example:   static unsigned long LastTime;
                if (TickEveryXms(&LastTime, 1000)) {//Code to run}    */
  static unsigned long _Middle = -1;                            //We just need a really big number, if more than 0 and less than this amount of ms is passed, return true)
  if (_Middle == -1) _Middle = _Middle / 2;                     //Somehow declairing middle on 1 line does not work
  if (millis() - (*_LastTime + _Delay) <= _Middle) {            //If it's time to update (keep brackets for overflow protection). If diffrence between Now (millis) and Nextupdate (*_LastTime + _Delay) is smaller than a big number (_Middle) then update. Note that all negative values loop around and will be really big (for example -1 = 4,294,967,295)
    *_LastTime = millis();                                      //Set new LastTime updated
    return true;
  }
  return false;
}
bool StringIsDigit(String IN, char IgnoreCharA = '0', char IgnoreCharB = '0');
bool StringIsDigit(String IN, char IgnoreCharA, char IgnoreCharB) {
  //IgnoreChar can be used to ignore ',' or '.' or '-'
  for (byte i = 0; i < IN.length(); i++) {
    if (isDigit(IN.charAt(i))) {                                //If it is a digit, do nothing
    } else if (IN.charAt(i) == IgnoreCharA) {                   //If it is IgnoreCharA, do nothing
    } else if (IN.charAt(i) == IgnoreCharB) {                   //If it is IgnoreCharB, do nothing
    } else {
      return false;
    }
  }
  return true;
}
String IngredientIDtoString(byte IN) {
  if (IN <= Ingredient_Amount)
    return IngredientS[IN];
  return "UNK";
}
byte IngredientStringToID(String IN) {
  if (StringIsDigit(IN)) {
    if (IN.toInt() < Ingredient_Amount)
      return IN.toInt();
    else
      return 0;
  }
  IN.trim();
  IN.toUpperCase();
  for (byte i = 0; i < Ingredient_Amount; i++) {
    if (IN == IngredientS[i])
      return i;
  }
  return 0;
}
byte GetDispenserID(byte IngredientID) {
  //  Serial.println("GetDispenserID " + String(IngredientID) + "(" + IngredientIDtoString(IngredientID) + ")");
  if (IngredientID == 0) return 0;
  for (byte i = 0; i <= Ingredient_Amount; i++) {
    if (Dispensers[i].IngredientID == IngredientID) {       //If the drink is found and avalible
      return i;
    }
  }
  return 0;
}
void MyYield() {
  WiFiManager.RunServer();                                    //Do WIFI server stuff if needed
  //FastLED.delay(1);
  yield();
}
void MyDelay(int DelayMS) {                                     //Just a non-blocking delay
  //DelayMS, delay in ms like in the Arduino Delay() function
  unsigned long _StartTime = millis();
  while (millis() < _StartTime + DelayMS)
    MyYield();
}
bool MoveWait(AccelStepper Step, byte RefferenceButton) {
  bool WaitMore = true;
  while (WaitMore) {
    if (not Step.run()) {                                       //If we have done all steps
      Step.setCurrentPosition(0);
      return false;
    } else if (digitalRead(RefferenceButton) == LOW) {          //If we have hit the switch
      Step.setCurrentPosition(0);
      return true;
    }
    yield();
  }
  return false;
}
bool Home(bool X = true, bool Y = true, bool Z = true);
bool Home(bool X, bool Y, bool Z) {
  Serial.println("Homeing");
  digitalWrite(PDO_Step_enable, LOW);                           //Enable all stepper drivers
  bool Homed_X = false, Homed_Y = false, Homed_Z = false;
  if (X) {
    Serial.println("X");
    Stepper_X.moveTo(-BedSize_X);
    if (MoveWait(Stepper_X, PDI_X_Ref)) {                       //If the switch is touched
      Stepper_X.moveTo(HomedistanceBounce);
      while (Stepper_X.run())
        MyYield();
      Stepper_X.setMaxSpeed(HomeMAXSpeed);
      Stepper_X.moveTo(0);
      Homed_X = MoveWait(Stepper_X, PDI_X_Ref);
      Stepper_X.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  if (Y) {
    Serial.println("Y");
    Stepper_Y.moveTo(-BedSize_Y);
    if (MoveWait(Stepper_Y, PDI_Y_Ref)) {                       //If the switch is touched
      Stepper_Y.moveTo(HomedistanceBounce);
      while (Stepper_Y.run())
        MyYield();
      Stepper_Y.setMaxSpeed(HomeMAXSpeed);
      Stepper_Y.moveTo(0);
      Homed_Y = MoveWait(Stepper_Y, PDI_Y_Ref);
      Stepper_Y.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  if (Z) {
    Serial.println("Z");
    Stepper_Z.moveTo(-BedSize_Z);
    if (MoveWait(Stepper_Z, PDI_Z_Ref)) {                       //If the switch is touched
      Stepper_Z.moveTo(HomedistanceBounce);
      while (Stepper_Z.run())
        MyYield();
      Stepper_Z.setMaxSpeed(HomeMAXSpeed);
      Stepper_Z.moveTo(0);
      Homed_Z = MoveWait(Stepper_Z, PDI_Z_Ref);
      Stepper_Z.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  if (X == Homed_X and Y == Homed_Y and Z == Homed_Z) {
    Homed = true;
    return true;
  }
  Serial.println(Homed);
  return Homed;
}
void Move(AccelStepper Step, int Pos, int Max) {
  if (Pos < 0) {
    Pos = 0;
    Serial.print("Error move out of bouds, moving to MIN instead");
  } else if (Pos > Max) {
    Pos = Max;
    Serial.print("Error move out of bouds, moving to MAX instead");
  }
  if (!Homed)
    Home();
  Step.moveTo(Pos);
}
void DisableSteppers() {
  Homed = false;
  digitalWrite(PDO_Step_enable, HIGH);                          //Disable all stepper drivers
}
//byte IsAvailble(Ingredient IN){
//  return GetDispenserID(IN.ID);
//}
bool IsAvailable(byte DrinkID) {
  Serial.println("Checking IsAvailable " + String(DrinkID));
  if (DrinkID > Drinks_Amount) return false;                    //If a invalid drink is given
  for (byte i = 0; i < 8; i++) {                                //For each Ingredient
    if (Drinks[DrinkID].Ingredients[i].ID != 0) {               //If there is a ingredient we should get
      if (GetDispenserID(Drinks[DrinkID].Ingredients[i].ID) == 0)
        return false;                                           //If it has no location, mark it as not available
    }
  }
  return true;
}
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3])  ;
}
bool IsTrue(String input) {
  input.toLowerCase();
  if (input == "1" or input == "true" or input == "yes" or input == "high")
    return true;
  return false;
}
String IsTrueToString(bool input) {
  if (input)
    return "TRUE";
  return "FALSE";
}
