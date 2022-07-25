#include <SoftwareSerial.h>

#define ledPin 13
unsigned long lastTransmission;
const int interval = 1000;

unsigned long preTXtime;
unsigned long TXtime;
SoftwareSerial loraDown(2,3);
SoftwareSerial loraUp(9,10);

void setup(){
    Serial.begin(9600);
    loraDown.begin(9600);
    delay(100);
    loraDown.println("AT+PARAMETER=10,7,1,7");
    loraDown.flush();
    loraDown.println("AT+ADDRESS=77");
    loraDown.flush();
    loraDown.println("AT+NETWORKID=2");
    loraDown.flush();
}

void loop()
{
     /* 
      TXtime = millis();
      if(TXtime-preTXtime >1000)
      {
        loraUp.println("AT+SEND=75,8,Testing!");
        loraUp.flush();
        preTXtime = TXtime;
      }
      */
      
      if(loraDown.available())
      {
        while(loraDown.available()) Serial.write(loraDown.read());
      }
      
      delay(100);
}
