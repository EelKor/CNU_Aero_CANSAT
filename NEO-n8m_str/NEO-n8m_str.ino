#include <SoftwareSerial.h>

SoftwareSerial GPS(2,3);  // Lora TX Lora Rx
byte buff[100];
 String lat;
  String lng;
  String positionFix ;

  
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
      Serial.println(msg);
    int index = msg.indexOf(",");// 첫 번째 콤마 위치
     int index2= msg.indexOf(",",index+1); 
     int index3 = msg.indexOf(",",index2+1); 
     int index4 = msg.indexOf(",",index3+1); 
     int index5 = msg.indexOf(",",index4+1);
     int index6 = msg.indexOf(",",index5+1);  
      positionFix = msg.substring(index6+1,index6+2);
      if (index6<43){positionFix = "0";}
      Serial.println(String(index)+'\t'+String(index2)+'\t'+String(index3)+'\t'+String(index4)+'\t'+String(index5)+'\t'+String(index6)+'\t'+positionFix/*+'\t'+lat+'\t'+lng*/);

     if(positionFix != "0"){   
      lat = msg.substring(17,29);
      lng = msg.substring(30,43);
     }

     else{
      lat = '0'; lng = '0';
    }
    }

    
  }
}
