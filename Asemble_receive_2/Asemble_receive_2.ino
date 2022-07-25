#include <SoftwareSerial.h>

const int interval = 1000;
SoftwareSerial TXlora(2,3);  // trans Lora
SoftwareSerial RXlora(9,10);  // receive Lora
String cmd;
  unsigned int preTXtime;
  unsigned int TXtime;

//라즈베리파이 시리얼 통신
//SoftwareSerial dataSerial(12,13);
void dataTX();
void dataRX();
String dataString;

/*================================================*/

void setup() {
Serial.begin(9600);


//라즈베리파이 시리얼 통신
      //dataSerial.begin(9600);
      
//lora
    TXlora.begin(9600);
    delay(100);
    TXlora.println("AT+PARAMETER=10,7,1,7"); delay(100);
    TXlora.println("AT+ADDRESS=76"); delay(100);
    TXlora.println("AT+NETWORKID=2"); delay(100);
      Serial.println("TXlora setup end");

    RXlora.begin(9600);
    delay(100);
    RXlora.println("AT+PARAMETER=10,7,1,7"); delay(100);
    RXlora.println("AT+ADDRESS=75"); delay(100); 
    RXlora.println("AT+NETWORKID=2"); delay(100);
      Serial.println("RXlora setup end");
}


void loop() {
  String inString;
   TXtime = millis();
    if(TXtime-preTXtime >1000){
   RXlora.listen();
    if(RXlora.available()){
      inString = String(RXlora.readStringUntil('\n'));
      delay(50);
    }
  
 if(inString.length() > 0)
  {
    Serial.println(inString);
  }

  
/*cmd = "test";
lora.println("AT+SEND=77,"+String(cmd.length())+","+cmd);
      delay(100);//*/

  
  TXlora.listen();
  if(Serial.available())
    {
     cmd = String(Serial.readStringUntil('\n'));
     
       if(cmd.length() > 0){
         TXlora.println("AT+SEND=77,"+String(cmd.length())+","+cmd);
         //Serial.println(cmd);
          delay(50);
       }
    }
    }
   //*/
}
