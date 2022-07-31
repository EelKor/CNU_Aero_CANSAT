 #include <SoftwareSerial.h>

unsigned long lastTransmission;
const int interval = 1000;
unsigned long TXtime;
unsigned long preTXtime;
String inString;
String dataString;


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
      if(TXtime-preTXtime > 1000 /*&& Serial.available()*/)
      { 
        String msg = Serial.readStringUntil('\n');
        //String testMsg = "-12,60,354443,00596";
        loraTX.println("AT+SEND=77," + String(msg.length()) + "," + msg);
        //loraTX.println("AT+SEND=77," + String(testMsg.length()) + "," + testMsg);
        
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
       inString =loraTX.readStringUntil('\n');
        loraTX.flush();
      }
  if(inString.length() > 0)
  {
    Serial.println(inString);
  }
      

}