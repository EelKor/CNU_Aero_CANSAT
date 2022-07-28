//수신부 코드입니다.

/* Lora_trans
  VCC 3.3V 
  tx 2      rx 3 */


#include <SoftwareSerial.h>
 
SoftwareSerial lora(2,3);
 
void setup()
{
  Serial.begin(9600);
  lora.begin(9600);
  lora.println("AT+PARAMETER=10,7,1,7");
  delay(100);
  lora.println("AT+IPR=9600"); //로라 속도
  delay(100);
  lora.println("AT+ADDRESS=77"); //로라 주소 지정
  delay(100);
  lora.println("AT+NETWORKID=2"); //네트워크 아이디
  delay(100);
  Serial.println("lora setup end");
 
}
 
void loop()
{
  String inString;
  String cmd;



//수신
  while(lora.available())
  {
    if(lora.available())
    {
     inString = String(lora.readStringUntil('\n'));
     delay(50);
    }
  }

  if(inString.length() > 0)
  {
    Serial.println(inString);
  }

  //송신
if(Serial.available())
    {
     cmd = String(Serial.readStringUntil('\n'));
     
       if(cmd.length() > 0){
         lora.println("AT+SEND=75,"+String(cmd.length())+","+cmd);
         //Serial.println(cmd);
          delay(50);
       }
    }


}