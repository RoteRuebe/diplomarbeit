void setup() {
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {
  digitalWrite(7, HIGH);
  delay(700);  
  digitalWrite(7, LOW);
  delay(700);

}
