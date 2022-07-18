#include <SoftwareSerial.h>

#define ledPin 13
unsigned long lastTransmission;
const int interval = 1000;
SoftwareSerial lora(2,3);  // Lora TX , Lora RX

void setup(){
    Serial.begin(115200);
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
    pinMode(ledPin,OUTPUT);
}

void loop()
{
      lora.println("AT+SEND=77,8,Testing!");
      while(lora.available())
      {
        Serial.write(lora.read());
      }

      delay(100);
}