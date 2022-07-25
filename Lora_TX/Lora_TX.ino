#include <SoftwareSerial.h>
 
const int interval = 1000;
SoftwareSerial lora(2,3);
unsigned int preTXtime;
unsigned int TXtime;

void setup(){
    Serial.begin(9600);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=77");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    //lora.println("AT+BAND=92000000");
    delay(100);
}

void loop()
{   /*TXtime = millis();
    if(TXtime-preTXtime >1000){
      lora.println("AT+SEND=75,8,Testing!"); //75한테 전송
      lora.flush();
      preTXtime = TXtime;
    }
    */
      while(lora.available())
      {
        Serial.write(lora.read()); //76이 보낸 것 읽어들임
      }

}
