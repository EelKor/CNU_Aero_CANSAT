  //lora
  
#include <SoftwareSerial.h>
unsigned long lastTransmission;
const int interval = 1000;
SoftwareSerial lora(2,3);  // Lora TX , Lora RX
String cmd;


void setup() {
 Serial.begin(9600);

    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7"); delay(100);
    lora.println("AT+ADDRESS=76"); delay(100);
    lora.println("AT+NETWORKID=2"); delay(100);
    //lora.println("AT+BAND=92000000"); delay(100);
}

void loop() {
  String inString;
    if(lora.available()){
      inString = String(lora.readStringUntil('\n'));
      delay(50);
    }
  
 if(inString.length() > 0)
  {
    Serial.println(inString);
  }

  
/*cmd = "test";
lora.println("AT+SEND=77,"+String(cmd.length())+","+cmd);
      delay(100);//*/
  if(Serial.available())
    {
     cmd = String(Serial.readStringUntil('\n'));
     
       if(cmd.length() > 0){
         lora.println("AT+SEND=77,"+String(cmd.length())+","+cmd);
         //Serial.println(cmd);
          delay(50);
       }
    }
   //*/
}
