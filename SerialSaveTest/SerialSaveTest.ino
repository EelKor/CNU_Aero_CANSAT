void setup() {
Serial.begin(9600);

}

void loop() {
while(Serial.available()){
  String inString = Serial.readStringUntil('\n');
  Serial.println(inString);
}
}
