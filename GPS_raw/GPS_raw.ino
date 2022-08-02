#include <SoftwareSerial.h>

SoftwareSerial GPS(2,3);  // Lora TX Lora Rx
byte buff[100];
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  GPS.begin(9600);
 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  
}

void loop() { // run over and over
  if (GPS.available()) {
        Serial.write(GPS.read());
    } 
}
