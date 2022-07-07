//수신부 코드입니다.

/* Lora_trans
  VCC 3.3V 
  tx 2      rx 3 */


#include <SoftwareSerial.h>
 
SoftwareSerial lora(2,3);
 
int val = 0;
 
void setup()
{
  Serial.begin(9600);
  lora.begin(9600);
  Serial.println("AT+IPR = 9600");
  Serial.println("AT+ADDRESS = 70");
  Serial.println("AT+NETWORKID = 70");
 
}
 
void loop()
{
  //수신부분
  String inString;
  while (lora.available())
  {
    if(lora.available()){
    inString = String(char(lora.read())); //전송받은 문자열
    }
  }
  if(inString.length()>0)
  {
    Serial.println(inString); //문자열 출력
  }

//전송부분
   lora.println("낙하산 전개 코드");
   while(lora.available()){
    Serial.write(lora.read()); //전송
  }
}
