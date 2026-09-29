const uint8_t PDO_Step = 12;
const uint8_t PDO_Dir = 13;
const uint8_t PDO_Step_enable = 14;
const uint8_t PDI_Light_Sensor = 15;

enum Modes {OFF, PRIME, RETRACT, UNLOAD};
//OFF= do nothing
//PRIME = Load until sensor is seen, than x turn backwards

void setup() {
  Serial.begin(115200);

  pinMode(PDI_Light_Sensor, INPUT);
  pinMode(PDO_Step, OUTPUT);
  pinMode(PDO_Dir, OUTPUT);
  pinMode(PDO_Step_enable, OUTPUT);
  digitalWrite(PDO_Dir, HIGH);         //Set direction counter clockwise
  digitalWrite(PDO_Step_enable, LOW);  //Enable (pin is inverted)

  int value = analogRead(15);  //Init value?
}

void loop() {
  static uint8_t mode = OFF;

  if (Serial.available()) {
    String Recieved = "";
    while (Serial.available()) {
      Recieved = Recieved + String(Serial.read(), DEC);
    }
    Serial.println("Serial recieved '" + Recieved + "'");

    if (Recieved == "P"){
      mode = PRIME;
    }
  }

  //Every 10s turn the motor on (or off it it was still running)
  static unsigned long LastTime2;
  static bool RunStepper = false;
  if (TickEveryXms(&LastTime2, 10000)) {
    RunStepper = !RunStepper;
  }

  //Run the motor if needed
  if (RunStepper) {
    static unsigned long lastStepTime = 0;
    static bool stepState = LOW;
    static const unsigned long STEP_HALF_PERIOD_US = 100;  // 5,000 steps/s
    if (micros() - lastStepTime >= STEP_HALF_PERIOD_US) {
      lastStepTime += STEP_HALF_PERIOD_US;
      stepState = !stepState;
      digitalWrite(PDO_Step, stepState);
    }
  }

  //Read sensor values
  static unsigned long LastTime;
  if (TickEveryXms(&LastTime, 50)) {
    int value = analogRead(15);
    //Calculate averga sensor reading
    const uint8_t SampleSize = 16;
    static int samples[SampleSize] = {};
    static int sampleIndex = 0;
    static long sampleTotal = 0;
    static int sampleCount = 0;
    sampleTotal -= samples[sampleIndex];
    samples[sampleIndex] = value;
    sampleTotal += value;
    sampleIndex++;
    if (sampleIndex >= SampleSize) sampleIndex = 0;
    if (sampleCount < SampleSize) sampleCount++;
    int average = sampleTotal / sampleCount;
    int AverageMin = average * 0.95;
    int AverageMax = average * 1.05;
    //Calcultate min value so the Arduino plot does not change as much
    static int minvalue = value;
    if (minvalue < value) minvalue = value;
    //Calcultate max value so the Arduino plot does not change as much
    static int maxvalue = value;
    if (value < maxvalue) maxvalue = value;
    
    //Print
    Serial.print("min:" + String(minvalue));
    Serial.print(",value:" + String(value));
    //Serial.print(",average:" + String(average));
    Serial.print(",max:" + String(maxvalue));
    Serial.print(",LimitUpper:" + String(AverageMax) + ", LimitLower:" + String(AverageMin));
    Serial.println();

    //Stop motor if new fluid or a runout is detected
    if ((value > AverageMax) or (value < AverageMin)) {
      RunStepper = false;
    }
  }
}

//Functions:
bool TickEveryXms(unsigned long *_LastTime, unsigned long _Delay) {
  //With overflow, can be adjusted, no overshoot correction, true when (Now < _LastTime + _Delay)
  /* Example:   static unsigned long LastTime;
                if (TickEveryXms(&LastTime, 1000)) {//Code to run}    */
  static unsigned long _Middle = -1;                  //We just need a really big number, if more than 0 and less than this amount of ms is passed, return true)
  if (_Middle == -1) _Middle = _Middle / 2;           //Somehow declairing middle on 1 line does not work
  if (millis() - (*_LastTime + _Delay) <= _Middle) {  //If it's time to update (keep brackets for overflow protection). If diffrence between Now (millis) and Nextupdate (*_LastTime + _Delay) is smaller than a big number (_Middle) then update. Note that all negative values loop around and will be really big (for example -1 = 4,294,967,295)
    *_LastTime = millis();                            //Set new LastTime updated
    return true;
  }
  return false;
}
