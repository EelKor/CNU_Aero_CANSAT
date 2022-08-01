#include <SoftwareSerial.h>

SoftwareSerial lora(2,3);  // RX, TX.

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=12,7,1,4");
    delay(100);
    lora.println("AT+ADDRESS=77");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    lora.println("AT+BAND=915000000");
    delay(100);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("LABEL,time,cmd,length,pa,altitude,FallSpeed,angle_y,angle_z,lat,lng,SOS,notice,.,.");
}

void loop() { // run over and over
  if (lora.available()) {
    String instring = "DATA,TIME,"+ String(lora.readStringUntil('\n'));
    Serial.println(instring);
  }
}
