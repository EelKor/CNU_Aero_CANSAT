 #include <SoftwareSerial.h>

#define ledPin 13
unsigned long lastTransmission;
const int interval = 1000;
unsigned long TXtime;
unsigned long preTXtime;

SoftwareSerial loraTX(2,3);  // Lora TX , Lora RX
SoftwareSerial loraRX(9,10);

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
      if(TXtime-preTXtime >1000)
      {
        loraTX.println("AT+SEND=77,19,CANSET SENDING 815!");
        loraTX.flush();
        preTXtime = TXtime;
      }

      if(loraTX.available())
       {
        while(loraTX.available())  Serial.write(loraTX.read());
      }
      

}
