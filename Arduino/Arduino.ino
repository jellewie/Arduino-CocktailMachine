/*
  Program written by JelleWho https://github.com/jellewie
  Board: https://dl.espressif.com/dl/package_esp32_index.json
*/

#include <AccelStepper.h>  //Make sure to install AccelStepper V1.64(+) manually //https://www.airspayce.com/mikem/arduino/AccelStepper/classAccelStepper.html#a68942c66e78fb7f7b5f0cdade6eb7f06

enum PumpMode { OFF,
                PRIME,
                UNLOAD,
                DISPENSE };
//OFF= do nothing
//PRIME = Load until sensor is seen, than 1 turn backwards
//UNLOAD = Run motor backwards for 30s to unload all fluid
//DISPENSE = Prime the dispense tube, than dispence the given amount, then retract the extension tube amount again (to avoid dripping)
const uint8_t SampleSize = 16;
const uint8_t PDO_Step = 12;  //Pin Digital Output, Motor step pin
const uint8_t PDO_Dir = 13;   //Motor Direction pin

struct PumpConfig {
  const uint8_t PDO_Step_enable;
  const uint8_t PDI_Light_Sensor;

  float mlPerRev;
  float PrimeRevolutions;
  PumpMode Mode = OFF;
  PumpMode LastMode = OFF;       //To see if there is an update to process
  bool FluidDetected = false;    //High if there is fluid detected, low if not
  bool FluidUpdateFlag = false;  //If there is a update to process
  uint32_t TargetSteps = 0;
  uint32_t CurrentSteps = 0;
  unsigned long ModeStartTime = 0;

  //Moving average
  uint16_t Samples[SampleSize] = {};  //To store all the measurements in
  uint8_t SampleIndex = 0;            //At what measurement we currently are
  int32_t SampleTotal = 0;            //Save the total value of all our measurements
};
const uint8_t PumpAmount = 1;
PumpConfig Pumps[PumpAmount] = {
  { 14, 15, 1.0, 5 }
};

AccelStepper Stepper(AccelStepper::DRIVER, PDO_Step, PDO_Dir);

void setup() {
  Serial.begin(115200);
  pinMode(PDO_Step, OUTPUT);
  pinMode(PDO_Dir, OUTPUT);
  Stepper.setMaxSpeed(7000);
  Stepper.setAcceleration(10000);
  for (uint8_t i = 0; i < PumpAmount; i++) {
    pinMode(Pumps[i].PDO_Step_enable, OUTPUT);
    pinMode(Pumps[i].PDI_Light_Sensor, INPUT);
  }
  Serial.println("booted");
}
void loop() {
  static unsigned long LastTimeHandleCommands;
  if (TickEveryXms(&LastTimeHandleCommands, 500))  //Run at muhc lower rate to save processing time
    HandleCommand();
  static unsigned long LastTimeReadSensors;
  if (TickEveryXms(&LastTimeReadSensors, 50))  //Run at lower rate to save processing time and make the sample time width bigger
    ReadSensors();

  UpdatePumps();

  Stepper.run();
}

void ChangeMode(uint8_t pumpId, PumpMode Mode) {
  Pumps[pumpId].Mode = Mode;
  Pumps[pumpId].ModeStartTime = millis();
  Pumps[pumpId].FluidUpdateFlag = true;
}
void HandleCommand() {  //HANDLE COMMANDS, (temp for serial testing)
  if (Serial.available()) {
    String received = Serial.readStringUntil('\n');
    received.trim();  // Removes \r, \n, spaces, etc.
    received.toUpperCase();
    Serial.println("Serial received '" + received + "'");
    if (received == "O") {
      ChangeMode(0, OFF);
    } else if (received == "P") {
      ChangeMode(0, PRIME);
    } else if (received == "U") {
      ChangeMode(0, UNLOAD);
    } else if (received == "E") {
      ChangeMode(0, DISPENSE);
    } else
      Serial.println("HandleCommand unknwown command '" + String(received) + "'");
  }
}
void ReadSensors() {  //Read the sensors and save the average
                      //The first average of SampleSize samples will be junk after a bootup.
  for (uint8_t i = 0; i < PumpAmount; i++) {
    PumpConfig& pump = Pumps[i];
    pump.SampleIndex++;                  //Move index forward
    if (pump.SampleIndex >= SampleSize)  //If we are about to overflow
      pump.SampleIndex = 0;              //Reset index
    //Calculate average sensor reading
    pump.SampleTotal -= pump.Samples[pump.SampleIndex];                  //Remove oldest measurement
    pump.Samples[pump.SampleIndex] = analogRead(pump.PDI_Light_Sensor);  //Set new measurement
    pump.SampleTotal += pump.Samples[pump.SampleIndex];                  //Add new measurement
    uint16_t Average = pump.SampleTotal / SampleSize;
    //Check if a fluid change has happened
    if (pump.Samples[pump.SampleIndex] > Average * 1.05) {         //If we went from no-fluid to fluid
      pump.FluidDetected = true;                                   //Update the value to reflect the detected change
      pump.FluidUpdateFlag = true;                                 //Flag there is an update
    } else if (pump.Samples[pump.SampleIndex] < Average * 0.95) {  //If we went from fluid to no-fluid
      pump.FluidDetected = false;                                  //Update the value to reflect the detected change
      pump.FluidUpdateFlag = true;                                 //Flag there is an update
    }
  }
}
void UpdatePumps() {
  for (uint8_t i = 0; i < PumpAmount; i++) {
    PumpConfig& pump = Pumps[i];
    if (pump.LastMode != pump.Mode)
      Serial.println("UpdatePumps pump=" + String(i) + " mode=" + String(pump.Mode));
    switch (pump.Mode) {
      case OFF:
        if (pump.LastMode != pump.Mode)  //If the mode just changed to OFF
          Stepper.stop();
        break;
      case PRIME:
        if (millis() - pump.ModeStartTime > 30000) {  //If there is no fluid detected after running, avoid flooding the whole place and stop
          Serial.println("ERROR, timeout Priming on pump " + String(i));
          pump.Mode = OFF;  //Cancel mode, we timed out
        } else if (pump.FluidUpdateFlag) {
          if (pump.FluidDetected and millis() - pump.ModeStartTime > 500)  //If fluid detected and running for a bit to avoid startup jerk
            MovePumpRevolutions(i, -1);                                    //Move 1 rotation backwards
          else
            MovePumpRevolutions(i, 100);  //Move forwards
          pump.FluidUpdateFlag = false;
        } else if (Stepper.isRunning() == false)  //If we reached our destination
          pump.Mode = OFF;                        //we are done
        break;
      case UNLOAD:
        if (pump.LastMode != pump.Mode)    //If the mode just changed to OFF
          MovePumpRevolutions(i, -100);     //Emthy the tube backwards
        if (Stepper.isRunning() == false)  //If we reached our destination
          pump.Mode = OFF;                 //we are done
        break;  //digitalWrite(PDO_Dir, HIGH);  //Set direction counter clockwise
      case DISPENSE:
        uint8_t dispenceAmountRotations = 20;
        uint8_t TubePrimeRotations = 4;
        if (pump.LastMode != pump.Mode)                                          //If the mode just changed to OFF
          MovePumpRevolutions(i, TubePrimeRotations + dispenceAmountRotations);  //Prime + dispence the tube to the glass
        static bool HasDispenced = false;
        if (Stepper.isRunning() == false)  //If we reached our destination
          if (HasDispenced) {
            pump.Mode = OFF;  //we are done
          } else {
            MovePumpRevolutions(i, TubePrimeRotations);  //Emthy the tube backwards
            HasDispenced = true;
          }
        break;
    }
    pump.LastMode = pump.Mode;
  }
}
void MovePumpRevolutions(uint8_t PumpId, float Revolutions) {
  for (uint8_t i = 0; i < PumpAmount; i++)
    digitalWrite(Pumps[i].PDO_Step_enable, HIGH);    //Disable all steppers
  digitalWrite(Pumps[PumpId].PDO_Step_enable, LOW);  //Enable this stepper
  const int StepsPerRev = 1600;                      //We have 1600 steps per full resolution
  long Steps = Revolutions * StepsPerRev;
  Stepper.move(Steps);
}

//Functions:
bool TickEveryXms(unsigned long* _LastTime, unsigned long _Delay) {
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
