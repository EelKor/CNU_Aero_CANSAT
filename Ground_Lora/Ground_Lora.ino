#include <SoftwareSerial.h>
//#define DEBUG
#define ledPin 13
unsigned long lastTransmission;
const int interval = 1000;

unsigned long preTXtime;
unsigned long TXtime;
SoftwareSerial loraDown(2,3);

void setup(){
    Serial.begin(9600);
    loraDown.begin(9600);
    delay(100);
    loraDown.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    loraDown.println("AT+ADDRESS=77");
    delay(100);
    loraDown.println("AT+NETWORKID=2");
    delay(100);
}

void loop()
{
      
      TXtime = millis();
      if(TXtime-preTXtime > 10000)
      {
        String msg = "동원령선포, 국방부장관";
        loraDown.println("AT+SEND=76," + String(msg.length()) + "," + msg);
        loraDown.flush();
        preTXtime = TXtime;
      }
      
      #ifdef DEBUG
      Serial.print(loraDown.listen());
      Serial.print(" ");
      Serial.print(loraDown.isListening());
      Serial.print(" ");
      Serial.print(loraDown.overflow());
      Serial.print(" ");
      Serial.println(loraDown.available());
      
      #endif
      if(loraDown.available())
      {
        Serial.println(loraDown.readStringUntil('\n'));
      }
      
}
