/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
    Internal 8Mhz
  Do an "Butn bootloader" for each new Attiny85 you program to burn in the clock settings!
  This code is debugable on an ESP32, when the compiler detects you are compiling to an ESP it will change the code to debugging
*/

uint8_t SlotID = 1;  //Unique 1-byte ID (change for every slot for dispenser). Must be higher than 0 and lower then 255

#ifdef ESP32
const uint8_t PDI_Slot_TXRX = 13;
#else
const uint8_t PDI_Slot_TXRX = 4;
const uint8_t LED_BUILTIN = 3;
#endif
const uint8_t Delaypulses = 5;      //Below 3 seems to be too unstable
const uint8_t DelayBeforeSend = 5;  //below 4 we start losing some pulses at the start
const uint8_t DelayAfterSend = 20;  //Must be higher then lastPulseTimeTimeout of DISPENSER
void setup() {
  pinMode(PDI_Slot_TXRX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
#ifdef ESP32
  SlotID = SlotID + 1;  //IDK seems the ATTiny always does not count the int, so just increase it to compensate
#endif
}
void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(PDI_Slot_TXRX, HIGH);
  pinMode(PDI_Slot_TXRX, INPUT);
  delay(DelayAfterSend);
  while (digitalRead(PDI_Slot_TXRX) == HIGH) {}  //Wait until its is pulled low by a dispenser
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PDI_Slot_TXRX, OUTPUT);
  delay(DelayBeforeSend);
  for (uint8_t i = 0; i < SlotID; i++) {  //Amount of pulses based on SlotID
    digitalWrite(PDI_Slot_TXRX, LOW);
    delayMicroseconds(Delaypulses);
    digitalWrite(PDI_Slot_TXRX, HIGH);
    delayMicroseconds(Delaypulses);
  }
}
