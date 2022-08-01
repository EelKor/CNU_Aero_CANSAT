#include <SoftwareSerial.h>

SoftwareSerial lora(6,5);  // RX, TX.

void setup() {
  Serial.begin(9600);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=12,7,1,4");
    delay(100);
    lora.println("AT+ADDRESS=78");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    lora.println("AT+BAND=815000000");
    delay(100);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  
  lora.println("AT");
}

void loop() { // run over and over
if (lora.available()) {
    Serial.write(lora.read());
  }
  if (Serial.available()) {
    String instring = String(Serial.readStringUntil('\n'));
    lora.println("AT+SEND=75,"+String(instring.length())+','+instring);
    Serial.println("AT+SEND=75,"+String(instring.length())+','+instring);
  }
}
