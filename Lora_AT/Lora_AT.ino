#include <SoftwareSerial.h>

SoftwareSerial LoRa(2, 3);  // RX, TX.

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  LoRa.begin(9600);
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  
  LoRa.println("AT");
}

void loop() { // run over and over
  if (LoRa.available()) {
    Serial.write(LoRa.read());
  }
  if (Serial.available()) {
    LoRa.write(Serial.read());
  }
}
