/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
    Internal 8Mhz
  Do an "Burn bootloader" for each new Attiny85 you program to burn in the clock settings!
*/

uint8_t SlotID = 1;  //Unique 1-byte ID (change for every slot for dispenser). Must be higher than 0 and lower then 255

const uint8_t PDI_Slot_TXRX = 4;
const uint8_t Delaypulses = 1;      //Must be lower then lastPulseTimeTimeout/2
const uint16_t DelayBeforeSend = 500;
const uint8_t DelayAfterSend = 20;  //Must be higher then lastPulseTimeTimeout
void setup() {
  pinMode(PDI_Slot_TXRX, OUTPUT);
#ifdef ESP32
  SlotID = SlotID + 1;  //IDK seems the ATTiny always does not count the int, so just increase it to compensate
#endif
}
void loop() {
  digitalWrite(PDI_Slot_TXRX, HIGH);
  pinMode(PDI_Slot_TXRX, INPUT);
  delay(DelayAfterSend);
  while (digitalRead(PDI_Slot_TXRX) == HIGH) {}  //Wait until its is pulled low by a dispenser
  pinMode(PDI_Slot_TXRX, OUTPUT);
  delayMicroseconds(DelayBeforeSend);
  for (uint8_t i = 0; i < SlotID; i++) {  //Amount of pulses based on SlotID
    digitalWrite(PDI_Slot_TXRX, LOW);
    delayMicroseconds(Delaypulses);
    digitalWrite(PDI_Slot_TXRX, HIGH);
    delayMicroseconds(Delaypulses);
  }
}
