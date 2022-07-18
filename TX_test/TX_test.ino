#include <SoftwareSerial.h>

SoftwareSerial lora(2,3);

void setup() {
    Serial.begin(9600);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=70");
    delay(100);
    lora.println("AT+NETWORKID=7");
    delay(100);
   // lora.println("AT+BAND=92000000");
   // delay(100);
}

void loop() {
  String str = "lora_ok";
  String cmd = "AT+SEND=77,"+String(str.length())+","+str;

 lora.println(cmd);
  while(lora.available())
  {
   Serial.write(lora.read());
  }
  Serial.println(cmd);

}
