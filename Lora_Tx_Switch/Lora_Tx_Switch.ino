#include <SoftwareSerial.h>
#define DEBUG
#define SWITCH 8

unsigned long lastTransmission;
const int interval = 1000;
int ignition = 1;
int prev_ignition = 0;
SoftwareSerial lora(2,3);

void setup(){
    Serial.begin(115200);

    // Lora 모듈 초기화
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=76");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    //lora.println("AT+BAND=92000000");
    delay(100);

    // 핀 선언
    pinMode(SWITCH, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{ 
  String cmd = "AT+SEND=77,1,";

  prev_ignition = ignition;
  ignition += digitalRead(SWITCH);

  if(ignition > 10)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    lora.println(cmd+String(1));
  }

  if(prev_ignition == ignition)
  {
    ignition = 0;
    prev_ignition = 0;
  }


  #ifdef DEBUG
  Serial.print(ignition);
  Serial.print(" , ");
  Serial.println(prev_ignition);
  #endif

  while(lora.available())
  {
    Serial.write(lora.read());
  }
  delay(50);

}
