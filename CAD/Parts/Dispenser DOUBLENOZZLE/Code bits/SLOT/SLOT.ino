/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
    Internal 8Mhz
  
  This code is bebugable on an ESP32, when the compiler detects you are compiling to an ESP it will change the code to debugging
*/

uint8_t SlotID = 1;  //Unique 1-byte ID (change for every slot for dispenser). Must be higher than 0

#ifdef ESP32
const uint8_t PDI_Slot_TXRX = 13;
#else
const uint8_t PDI_Slot_TXRX = 4;
#endif
const uint8_t Delaypulses = 5;      //Below 3 seems to be too unstable
const uint8_t DelayBeforeSend = 5;  //below 4 we start losing some pulses at the start
const uint8_t DelayAfterSend = 20;  //Must be higher then lastPulseTime timeout of DISPENSER
void setup() {
  pinMode(PDI_Slot_TXRX, OUTPUT);
#ifdef ESP32
  pinMode(LED_BUILTIN, OUTPUT);
#else
  SlotID = SlotID + 1;  //IDK seems the ATTiny always does not count the int, so just increase it to compensate
#endif
}
void loop() {
#ifdef ESP32
  digitalWrite(LED_BUILTIN, LOW);
#endif
  digitalWrite(PDI_Slot_TXRX, HIGH);
  pinMode(PDI_Slot_TXRX, INPUT);
  delay(DelayAfterSend);
  while (digitalRead(PDI_Slot_TXRX) == HIGH) {}  //Wait until its is pulled low by a dispenser
#ifdef ESP32
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  pinMode(PDI_Slot_TXRX, OUTPUT);
  delay(DelayBeforeSend);
  for (uint8_t i = 0; i < SlotID; i++) {  //Amount of pulses based on SlotID
    digitalWrite(PDI_Slot_TXRX, LOW);
    delayMicroseconds(Delaypulses);
    digitalWrite(PDI_Slot_TXRX, HIGH);
    delayMicroseconds(Delaypulses);
  }
}
