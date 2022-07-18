#include <SoftwareSerial.h>

SoftwareSerial lora(2,3);

void setup() {
  Serial.begin(9600);
  lora.begin(9600);
  Serial.println("lora setup");
  Serial.println("AT+IPR = 9600"); //로라 속도
  Serial.println("AT+ADDRESS = 70"); //로라 주소 지정
  Serial.println("AT+NETWORKID = 7"); //네트워크 아이디
  Serial.println("lora setup end");
}

void loop() {
  String str = "lora ok";
  String cmd = "AT+SEND=77,1,";

  if(lora.available())
  {
    lora.println(cmd+str);
  }

}
