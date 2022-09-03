struct Ingredient {
  byte ID;
  String Action;                                                //If given, prompt the message and wait for user confirmation first
  byte ml;
};
struct Drink {
  String Name;
  int Color;
  Ingredient Ingredients[8];
};
struct Dispenser {
  byte Type;
  unsigned int LocationX;
  unsigned int LocationY;
  unsigned int LocationZ;                                                //Used for Pump_ID for PUMP
  unsigned int TimeMSML;                                                 //Timer per ms for each mL
  unsigned int TimeMSoff;                                                //Delay in MS to wait after
  byte IngredientID;
};
enum TypeE {UNSPECIFIED, SHOTDISPENSER, PUMP};
String TypeS[] {"UNSPECIFIED", "SHOTDISPENSER", "PUMP"};

byte Ingredient_Amount = sizeof(IngredientS) / sizeof(IngredientS[0]);//Why filling this in if we can automate that? :)
byte Type_Amount = sizeof(TypeS) / sizeof(TypeS[0]);//Why filling this in if we can automate that? :)
Dispenser Dispensers[Dispensers_Amount] = {
  //Type        , X     , Y     , Z    , MSml, MSoff, IngredientID
  {PUMP         , 3000  , 0     , 1    , 500 , 500  , 0},
  {SHOTDispenser, 350   , 0     , 7350 , 100 , 1000 , 1},
  {SHOTDispenser, 4310  , 0     , 7350 , 100 , 1000 , 2},
  {SHOTDispenser, 8270  , 0     , 7350 , 100 , 1000 , 3},
  {SHOTDispenser, 12230 , 0     , 7350 , 100 , 1000 , 4},
  {SHOTDispenser, 16190 , 0     , 7350 , 100 , 1000 , 5},
  {SHOTDispenser, 20150 , 0     , 7350 , 100 , 1000 , 6},
  {SHOTDispenser, 350   , 7250  , 7350 , 100 , 1000 , 7},
  {SHOTDispenser, 4310  , 7250  , 7350 , 100 , 1000 , 8},
  {SHOTDispenser, 8270  , 7250  , 7350 , 100 , 1000 , 9},
  {SHOTDispenser, 12230 , 7250  , 7350 , 100 , 1000 , 10},
  {SHOTDispenser, 16190 , 7250  , 7350 , 100 , 1000 , 11},
  {SHOTDispenser, 20150 , 7250  , 7350 , 100 , 1000 , 12}
};

/*
  //==================================================
  //Basic universal LED functions. These includes start postion, amount (inc overflow correction and such)
  //==================================================
  void LED_Fill(int From, int Amount, CRGB Color, int MaxBound = TotalLEDs);
  void LED_Fill(int From, int Amount, CRGB Color, int MaxBound) {
  while (From >= MaxBound) From -= MaxBound;                    //(Protection out of array bounds) Loop the LEDs around (TotalLEDs+1 is the same as LED 1)
  if (Amount > MaxBound) Amount = MaxBound;                     //(Protection out of array bounds) if more LEDs are given than there are in the array, set the amount to all LEDs
  if (From + Amount > MaxBound) {                               //Overflow protection
    byte calc1 = MaxBound - From;                               //Calculates the amount of LEDs which need to on on the end of the strip
    fill_solid(&(LEDs[From]), calc1, Color);
    fill_solid(&(LEDs[0]), Amount - calc1, Color);
  } else
    fill_solid(&(LEDs[From]), Amount, Color);
  }
  void LED_Add(int From, int Amount, CRGB Color, int MaxBound = TotalLEDs);
  void LED_Add(int From, int Amount, CRGB Color, int MaxBound) {
  while (From >= MaxBound) From -= MaxBound;                    //(Protection out of array bounds) Loop the LEDs around (TotalLEDs+1 is the same as LED 1)
  if (Amount > MaxBound) Amount = MaxBound;                     //(Protection out of array bounds) if more LEDs are given than there are in the array, set the amount to all LEDs
  if (From + Amount > MaxBound) {                               //Overflow protection
    byte calc1 = MaxBound - From;                               //Calculates the amount of LEDs which need to on on the end of the strip
    for (int i = From; i < From + calc1; i++)
      LEDs[i] += Color;
    for (int i = 0; i < Amount - calc1; i++)
      LEDs[i] += Color;
  } else
    for (int i = From; i < From + Amount; i++)
      LEDs[i] += Color;
  }
  void LED_Move(int From, int Amount, CRGB Color, byte Sets, byte Length, byte *Counter, bool Reverse = false, bool Reset = true, int MaxBound = TotalLEDs);
  void LED_Move(int From, int Amount, CRGB Color, byte Sets, byte Length, byte *Counter, bool Reverse, bool Reset, int MaxBound) {
  //From = The first LED to do the animation on
  //Amount = The amount of LEDS to do the animation on
  //Color = the RGB value to use
  //Sets = The amount of sets going around
  //Length = How long each set would be
  //Counter = a pointer to a counter where we keep track of how far we are
  //Reverse = Reverse the animation direction
  //Reset = reset the LED color with every call (do not use if using overlapping moves)
  //Example:
  //  static byte counter;
  //  Move(5, 10, CRGB(0, 1, 0), 2, 2, &counter);

  byte Count = *Counter;
  if (Reverse)
    Count = Amount - Count - 1;
  if (Reset)
    LED_Fill(From, Amount, CRGB(0, 0, 0), MaxBound);
  byte poss[Sets];                                              //Array for saving the positions of the sections
  for (byte i = 0; i < Sets; i++) {                             //Beginning of the loop which will send each position and length
    poss[i] = Count + Amount * i / Sets;                        //This will calculate each position by adding the offset times the position number to the first position
    byte posX;                                                  //This is the variable which will be used for sending position start. (this can overflow above TotalLEDs, but this will be fixed by the Fill command)
    if (poss[i] >= Amount) {                                    //To see if the position is to bigger than the total amount
      posX = From + poss[i] - Amount;                           //Subtract the total amount of LEDs from the position number
    } else {                                                    //Otherwise it will just use the position data without modifying it
      posX = From + poss[i];                                    //Just use the position number
    }
    if (posX <= From + Amount - Length) {                       //If the whole section ends before the total amount is reached it will just us the normal way of setting the LEDs
      LED_Fill(posX, Length, Color, MaxBound);                  //With the standard fill solid command from FastLED, LEDs[posX] PosX stands for beginning position, Amount will stand for the size of the sections and the last one is the color
    } else {
      byte calc1 = (From + Amount) - posX;                      //Calculates the amount of LEDs which need to be set from the beginning
      LED_Fill(posX, calc1, Color, MaxBound);                   //Fills the LEDs at the end of the strip
      LED_Fill(From, Length - calc1, Color, MaxBound);          //Fills the LEDs at the beginning of the strip
    }
  }
   Counter += 1;
  if (*Counter >= Amount)
     Counter = *Counter - Amount;
  }
  bool LED_Flash(int From, int Amount, CRGB Color, CRGB Color2 = CRGB(0, 0, 0), int MaxBound = TotalLEDs);
  bool LED_Flash(int From, int Amount, CRGB Color, CRGB Color2, int MaxBound) {
  if (LEDs[From] != Color) {
    LED_Fill(From, Amount, Color, MaxBound);
    return true;
  }
  LED_Fill(From, Amount, Color2, MaxBound);
  return false;
  }
  void LED_Rainbow(int From, int Amount, byte DeltaHue, int MaxBound = TotalLEDs);
  void LED_Rainbow(int From, int Amount, byte DeltaHue, int MaxBound) {
  //byte DeltaHue = Diffrence between each LED in hue
  static byte gHue;
  gHue++;
  while (From >= MaxBound) From -= MaxBound;                    //(Protection out of array bounds) Loop the LEDs around (TotalLEDs+1 is the same as LED 1)
  if (Amount > MaxBound) Amount = MaxBound;                     //(Protection out of array bounds) if more LEDs are given than there are in the array, set the amount to all LEDs
  if (From + Amount > MaxBound) {                               //Overflow protection
    byte calc1 = MaxBound - From;                               //Calculates the amount of LEDs which need to on on the end of the strip
    fill_rainbow(&(LEDs[From]), calc1, gHue, DeltaHue);
    fill_rainbow(&(LEDs[0]), Amount - calc1, gHue, DeltaHue);
  } else
    fill_rainbow(&LEDs[From], Amount, gHue, DeltaHue);
  }
  void LED_Wobble(int From, int Amount, CRGB Color, byte Sets, byte Length, int MaxBound = TotalLEDs);
  void LED_Wobble(int From, int Amount, CRGB Color, byte Sets, byte Length, int MaxBound) {
  //Sort of a move, but just back and forth between the start en end
  static byte Counter;                                          //this function can only be called once, this 'Counter' is a 1 time counter (could not get the pointer working to attach it to the caller)
  static bool Reverse = false;
  LED_Move(From, Amount, Color, Sets, Length, &Counter, Reverse, MaxBound);
  if (Reverse) {
    if (Counter == Amount - 1) {
      Reverse = false;
      Counter = 0;
    }
  } else {
    if (Counter == Amount - Length) {
      Reverse = true;
      Counter = Length - 1;
    }
  }
  }
  void LED_Blink(int From, int Amount, CRGB rgb, byte AlwaysOn, byte * Counter, bool Reverse = false, bool Reset = true, int MaxBound = TotalLEDs);
  void LED_Blink(int From, int Amount, CRGB rgb, byte AlwaysOn, byte * Counter, bool Reverse, bool Reset, int MaxBound) {
  if (Reset)
    LED_Fill(From, Amount, CRGB(0, 0, 0), MaxBound);            //Turn LEDs off
  if (Reverse) {
    LED_Fill(From + Amount - AlwaysOn, AlwaysOn, rgb, MaxBound); //Set some LEDs to be always on
    LED_Fill(From + Amount - *Counter, *Counter, rgb, MaxBound); //Set the counter amount of LEDs on (this will increase)
  } else {
    LED_Fill(From, AlwaysOn, rgb, MaxBound);                    //Set some LEDs to be always on
    LED_Fill(From, *Counter, rgb, MaxBound);                    //Set the counter amount of LEDs on (this will increase)
  }
   Counter += 1;                                                //This will make the blink 1 longer each time
  if (*Counter > Amount)                                        //If we are at max length
     Counter = 0;                                               //Reset counter
  }
  void LED_BackAndForth(int From, int Amount, CRGB Color, byte *Counter, bool *Direcion, bool Reverse = false, bool Reset = true, int MaxBound = TotalLEDs);
  void LED_BackAndForth(int From, int Amount, CRGB Color, byte *Counter, bool *Direcion, bool Reverse, bool Reset, int MaxBound) {
  //Fills then emties the range of leds one by one
  if (Reset)
    LED_Fill(From, Amount, CRGB(0, 0, 0), MaxBound);

  if (*Direcion)
     Counter -= 1;                                              //This will make the animation_on 1 shorter each time
  else
     Counter += 1;                                              //This will make the animation_on 1 longer each time
  if (*Counter >= Amount or * Counter == 0)                     //If we are at max length or at the start
     Direcion = !*Direcion;                                     //Flip direction

  if (Reverse)
    LED_Fill(From + Amount - *Counter, *Counter, Color, MaxBound); //Set the counter amount of LEDs on
  else
    LED_Fill(From, *Counter, Color, MaxBound);                  //Set the counter amount of LEDs on
  }
  //==================================================


  //*/
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
bool SetDispenser(Dispenser Dis, byte i) {
  if (i >= Dispensers_Amount)
    return false;
  Dispensers[i] = Dis;
  return true;
}
bool AddDispenser(Dispenser Dis) {
  for (byte i = 0; i < Dispensers_Amount; i++) {
    if (Dispensers[i].Type != UNSPECIFIED) {
      Dispensers[i] = Dis;
      return true;
    }
  }
  return false;
}
//bool RemoveDispenser(byte IngredientID) {
//  for (byte i = 0; i < Dispensers_Amount; i++) {
//    if (Dispensers[i].IngredientID == IngredientID) {
//      if (i != 0) {                                             //If this is the first one (needs to be ocupied)
//        Dispensers[i].Type = UNSPECIFIED;
//      } else {
//        for (byte j = 1; j < Dispensers_Amount; j++) {
//          if (Dispensers[j].Type != UNSPECIFIED) {
//            Dispensers[i] = Dispensers[j];
//            Dispensers[j].Type = UNSPECIFIED;
//          }
//        }
//      }
//      return true;
//    }
//  }
//  return false;
//}
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
String TypeIDtoString(byte IN) {
  if (IN <= Type_Amount)
    return TypeS[IN];
  return "UNK";
}
byte TypeStringToID(String IN) {
  if (StringIsDigit(IN)) {
    if (IN.toInt() < Type_Amount)
      return IN.toInt();
    else
      return 0;
  }
  IN.trim();
  IN.toUpperCase();
  for (byte i = 0; i < Type_Amount; i++) {
    if (IN == TypeS[i]) {
      Serial.println(i);
      return i;
    }
  }
  return 0;
}
byte GetDispenserID(byte IngredientID) {
  //  Serial.println("GetDispenserID " + String(IngredientID) + "(" + IngredientIDtoString(IngredientID) + ")");
  if (IngredientID == 0) return 0;
  for (byte i = 0; i <= Ingredient_Amount; i++) {
    if (Dispensers[i].IngredientID == IngredientID) {           //If the drink is found and avalible
      return i;
    }
  }
  return 0;
}
void DisableSteppers() {
  Homed = false;
  Stepper_X.moveTo(Stepper_X.currentPosition());
  Stepper_Y.moveTo(Stepper_Y.currentPosition());
  Stepper_Z.moveTo(Stepper_Z.currentPosition());
  digitalWrite(PDO_Step_enable, HIGH);                          //Disable all stepper drivers
}
void CheckEEPROMSave() {
  if (SaveEEPROMinSeconds >= 0) {
    static unsigned long LastTime;
    if (TickEveryXms(&LastTime, 1000)) {
      if (SaveEEPROMinSeconds == 0)
        WiFiManager.WriteEEPROM();                                  //(runtime) If you want to manually save the settings(EEPROM LIMITED WRITES! do not spam)
      SaveEEPROMinSeconds -= 1;
    }
  }
}
void CheckDisableSteppers() {
  if (DisableSteppersinSeconds >= 0) {
    static unsigned long LastTime;
    if (TickEveryXms(&LastTime, 1000)) {
      if (DisableSteppersinSeconds == 0)
        DisableSteppers();
      DisableSteppersinSeconds -= 1;
    }
  }
}
bool IsTrue(String input) {
  input.toLowerCase();
  if (input == "1" or input == "true" or input == "yes" or input == "high")
    return true;
  return false;
}
String IsTrueToString(bool input) {
  if (input)
    return "true";
  return "false";
}
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3])  ;
}
void LcdPrint(String msg = "", String msg2 = "");
void LcdPrint(String msg, String msg2) {
  Serial.println("LcdPrint:" + msg + "_" + msg2);
  if (msg != "") {
    for (byte i = msg.length(); i < 16; i++) {
      msg += " ";
    }
    lcd.setCursor(1, 0);
    lcd.print(msg);
  }
  if (msg2 != "") {
    for (byte i = msg2.length(); i < 16; i++) {
      msg2 += " ";
    }
    lcd.setCursor(1, 1);
    lcd.print(msg2);
  }
}
void MyYield() {
  WiFiManager.CheckAndReconnectIfNeeded(true);
  WiFiManager.RunServer();                                       //Do WIFI server stuff if needed
  CheckEEPROMSave();
  CheckDisableSteppers();
  static bool LastButtonState = HIGH;
  bool ButtonState = digitalRead(PDI_S);
  if (LastButtonState and !ButtonState)
    LcdPrint("Mixer ready!", IpAddress2String(WiFi.localIP()));
  LastButtonState = ButtonState;
  //FastLED.delay(1);
  yield();
}
void MyDelay(int DelayMS) {                                     //Just a non-blocking delay
  //DelayMS, delay in ms like in the Arduino Delay() function
  unsigned long _StartTime = millis();
  while (millis() < _StartTime + DelayMS)
    MyYield();
}
bool MoveWait(AccelStepper Step, byte RefferenceButton, int pos = 0);
bool MoveWait(AccelStepper Step, byte RefferenceButton, int pos) {
  Step.moveTo(pos);
  Serial.println("MW: " + String(pos) + " b=" + String(RefferenceButton) + "=" + String(digitalRead(RefferenceButton)));
  while (true) {
    Step.run();
    if (not Step.isRunning()) {                                 //If we have done all steps
      Step.setCurrentPosition(0);
      Serial.println("MW: End of steps");
      return false;
    } else {
      if (digitalRead(RefferenceButton) == LOW) {          //If we have hit the switch
        Step.setCurrentPosition(0);
        Serial.println("MW: Switch reached");
        return true;
      }
    }
    yield();
  }
}
bool Home(bool X = true, bool Y = true, bool Z = true);
bool Home(bool X, bool Y, bool Z) {
  LcdPrint("Homing", " ");
  digitalWrite(PDO_Step_enable, LOW);                           //Enable all stepper drivers
  bool Homed_X = false, Homed_Y = false, Homed_Z = false;
  if (Z) {
    LcdPrint("", "Z");
    Stepper_Z.setCurrentPosition(0);
    if (MoveWait(Stepper_Z, PDI_Z_Ref, BedSize_Z * -1.1)) {  //If the switch is touched
      Stepper_Z.moveTo(HomedistanceBounceZ);
      while (Stepper_Z.run())
        yield();
      Stepper_Z.setMaxSpeed(HomeMAXSpeed);
      Homed_Z = MoveWait(Stepper_Z, PDI_Z_Ref, -(BedSize_Z / 20));
      Stepper_Z.setCurrentPosition(0);                          //THERE IS A BUG IN AccelStepper SO WE NEED THIS SOMEHOW
      Stepper_Z.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  if (X) {
    LcdPrint("", "X");
    Stepper_X.setCurrentPosition(0);
    if (MoveWait(Stepper_X, PDI_X_Ref, BedSize_X  * -1.1)) {     //If the switch is touched
      Stepper_X.moveTo(HomedistanceBounce);
      while (Stepper_X.run())
        yield();
      Stepper_X.setMaxSpeed(HomeMAXSpeed);
      Homed_X = MoveWait(Stepper_X, PDI_X_Ref, -(BedSize_X / 20));
      Stepper_X.setCurrentPosition(0);                          //THERE IS A BUG IN AccelStepper SO WE NEED THIS SOMEHOW
      Stepper_X.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  if (Y) {
    LcdPrint("", "Y");
    Stepper_Y.setCurrentPosition(0);
    if (MoveWait(Stepper_Y, PDI_Y_Ref, BedSize_Y * -1.1)) {  //If the switch is touched
      Stepper_Y.moveTo(HomedistanceBounce);
      while (Stepper_Y.run())
        yield();
      Stepper_Y.setMaxSpeed(HomeMAXSpeed);
      Homed_Y = MoveWait(Stepper_Y, PDI_Y_Ref, -(BedSize_Y / 20));
      Stepper_Y.setCurrentPosition(0);                          //THERE IS A BUG IN AccelStepper SO WE NEED THIS SOMEHOW
      Stepper_Y.setMaxSpeed(MotorMAXSpeed);                     //Reset max speed
    }
  }
  LcdPrint("Homed", "X" + String(X) + "," + String(Homed_X) + " Y" + String(Y) + "," + String(Homed_Y) + " Z" + String(Z) + "," + String(Homed_Z));
  if (X == Homed_X and Y == Homed_Y and Z == Homed_Z) {
    Homed = true;
    return true;
  }
  return false;
}
void MoveTo(int LocationX = -1, int LocationY = -1, int LocationZ = -1);
void MoveTo(int LocationX, int LocationY, int LocationZ) {
  Serial.println("MoveTo " + String(LocationX) + " , " + String(LocationY) + " , " + String(LocationZ));
  if (!Homed) {
    if (!Home(true, true, true)) {
      return;
    }
  }
  if ((LocationX != -1 or LocationY != -1 ) and Stepper_Z.currentPosition() != 0 ) {//Always move Z down before moving XY
    if (LocationX != Stepper_X.currentPosition() or LocationY != Stepper_Y.currentPosition()) {//Unless we are already at XY
      Stepper_Z.moveTo(0);
      while (Stepper_Z.run()) {
        yield();
      }
    }
  }
  if (LocationX >= 0 and LocationX < BedSize_X * 10) {
    if (LocationX > BedSize_X)
      LocationX = BedSize_X;
    Stepper_X.moveTo(LocationX);
  }
  if (LocationY >= 0 and LocationY < BedSize_Y * 10) {
    if (LocationY > BedSize_Y)
      LocationY = BedSize_Y;
    Stepper_Y.moveTo(LocationY);
  }
  if (LocationZ >= 0 and LocationZ < BedSize_Z * 10) {
    if (LocationZ > BedSize_Z)
      LocationZ = BedSize_Z;
    Stepper_Z.moveTo(LocationZ);
  }
  bool WaitMore = true;
  while (WaitMore) {
    Stepper_X.run();
    Stepper_Y.run();
    if (!Stepper_X.isRunning() and !Stepper_Y.isRunning())
      WaitMore = false;
    yield();
  }
  while (Stepper_Z.run()) {//Always move Z after moving XY
    yield();
  }
}
#define TimeoutWaitingOnUserMs 5 * 60 * 1000
void WaitForUser(String msg, String msg2) {
  LcdPrint(msg, msg2);
  MoveTo(Manual_X, Manual_Y, BedSize_Z);
  bool WaitMore = true;
  while (WaitMore) {
    if (digitalRead(PDI_S) == LOW)
      WaitMore = false;
    static unsigned long LastTime;
    if (TickEveryXms(&LastTime, TimeoutWaitingOnUserMs))
      WaitMore = false;
    MyYield();
  }
  LcdPrint(msg, "User confirmed");
}
