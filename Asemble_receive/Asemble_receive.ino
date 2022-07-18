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
  Serial.println("lora setup");
  lora.println("AT+PARAMETER=10,7,1,7");
  delay(100);
  lora.println("AT+IPR = 9600"); //로라 속도
  delay(100);
  lora.println("AT+ADDRESS = 71"); //로라 주소 지정
  delay(100);
  lora.println("AT+NETWORKID = 2"); //네트워크 아이디
  delay(100);
  Serial.println("lora setup end");
  delay(100);
 
}
 
void loop()
{
  String inString;

  while(lora.available())
  {
    if(lora.available())
    {
       inString += String(char(lora.read()));
    }
  }

  if(inString.length() > 0)
  {
    Serial.print(inString);
    inString.remove(0);
  }

}
