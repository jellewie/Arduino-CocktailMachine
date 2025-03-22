/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
    Internal 8Mhz
  
  This code is bebugable on an ESP32, when the compiler detects you are compiling to an ESP it will change the code to debugging
*/

byte SlotID = 250;  //Unique 1-byte ID (change for every slot for dispenser). Must be higher than 0

#ifdef ESP32
const byte PDI_Slot_TXRX = 13;
#else
const byte PDI_Slot_TXRX = 4;
#endif
const byte Delaypulses = 1;
void setup() {
  pinMode(PDI_Slot_TXRX, OUTPUT);
#ifdef ESP32
  pinMode(LED_BUILTIN, OUTPUT);
#else
  SlotID = SlotID + 1; //IDK seems the ATTiny always does not count the int, so just increase it to compensate
#endif
}
void loop() {
#ifdef ESP32
  digitalWrite(LED_BUILTIN, LOW);
#endif
  digitalWrite(PDI_Slot_TXRX, HIGH);
  pinMode(PDI_Slot_TXRX, INPUT);
  delay(10);
  while (digitalRead(PDI_Slot_TXRX) == HIGH) {}  //Wait until its is pulled low by a dispenser
#ifdef ESP32
  digitalWrite(LED_BUILTIN, HIGH);
#endif
  pinMode(PDI_Slot_TXRX, OUTPUT);
  delay(2);
  for (byte i = 0; i < SlotID; i++) {  //Amount of pulses based on SlotID
    digitalWrite(PDI_Slot_TXRX, LOW);
    delayMicroseconds(Delaypulses);
    digitalWrite(PDI_Slot_TXRX, HIGH);
    delayMicroseconds(Delaypulses);
  }
}
