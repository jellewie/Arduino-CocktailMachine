void setup() {
  Serial.begin(115200);
  pinMode(15, INPUT);
}

void loop() {
  int value = analogRead(15);

  static int maxvalue = value;
  if (value < maxvalue) maxvalue = value;

  static int minvalue = value;
  if (minvalue < value) minvalue = value;

  Serial.print("min:" + String(minvalue));
  Serial.print(",value:" + String(analogRead(15)));
  Serial.print(",max:" + String(maxvalue));
  Serial.println();
  delay(100);
}