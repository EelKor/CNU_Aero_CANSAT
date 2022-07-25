 #include <SoftwareSerial.h>

#define ledPin 13
//#define DEBUG
unsigned long lastTransmission;
const int interval = 1000;
unsigned long TXtime;
unsigned long preTXtime;

SoftwareSerial loraTX(2,3);  // Lora TX , Lora RX

void setup(){
    Serial.begin(9600);
    delay(100);
    loraTX.begin(9600); 
    delay(100);
    loraTX.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    loraTX.println("AT+ADDRESS=76");
    delay(100);
    loraTX.println("AT+NETWORKID=2");
    delay(100);


}

void loop()
{
      TXtime = millis();
      if(TXtime-preTXtime > 1000 && Serial.available())
      { 
        String msg = Serial.readStringUntil('\n');
        loraTX.println("AT+SEND=77," + String(msg.length()) + "," + msg);
        loraTX.flush();
        preTXtime = TXtime;
      }

      #ifdef DEBUG
      Serial.print(loraTX.isListening());
      Serial.print(" ");
      Serial.println(loraTX.available());
      #endif
      
      if(loraTX.available())
       {
        Serial.println(loraTX.readStringUntil('\n'));
      }
      

}
