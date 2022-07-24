#include <SoftwareSerial.h>

SoftwareSerial GPS(5, 6);  // Lora TX Lora Rx
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
    GPS.readBytesUntil('\n',buff,100);
    String msg = buff;
    if(msg.substring(0,6).equals("$GNGGA") || msg.substring(0,6).equals("$GPGGA"))
    {
      String lat = msg.substring(17,29);
      String lng = msg.substring(30,43);
      Serial.print(lat);
      Serial.print("\t");
      Serial.println(lng);    
    }
    
    

  }
}
