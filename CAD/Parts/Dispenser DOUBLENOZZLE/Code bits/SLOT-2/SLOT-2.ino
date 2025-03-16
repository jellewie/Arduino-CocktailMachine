/*
  Program written by JelleWho https://github.com/jellewie
  Board: Attiny85
    Internal 8Mhz
*/
const byte SlotID = 250;  //Unique 1-byte ID (change for every slot for dispenser). must be higher than 0

const byte PDI_Slot_TXRX = 13;
const byte Delaypulses = 10;
void setup() {
  pinMode(PDI_Slot_TXRX, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(PDI_Slot_TXRX, HIGH);
  pinMode(PDI_Slot_TXRX, INPUT);
  delay(10);
  while (digitalRead(PDI_Slot_TXRX) == HIGH) {}	//Wait until its is pulled low by a dispenser
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(PDI_Slot_TXRX, OUTPUT);
  delay(10);
  for (byte i = 0; i < SlotID; i++) { //Amount of pulses based on SlotID
    digitalWrite(PDI_Slot_TXRX, LOW);
    delayMicroseconds(Delaypulses);
    digitalWrite(PDI_Slot_TXRX, HIGH);
    delayMicroseconds(Delaypulses);
  }
}
