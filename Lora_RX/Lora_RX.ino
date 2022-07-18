#include <SoftwareSerial.h>
 
SoftwareSerial lora(2,3);

void setup(){
    Serial.begin(9600);
    lora.begin(9600);
    delay(100);
    lora.println("AT+PARAMETER=10,7,1,7");
    delay(100);
    lora.println("AT+ADDRESS=77");
    delay(100);
    lora.println("AT+NETWORKID=2");
    delay(100);
    //lora.println("AT+BAND=92000000");
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
  }
}
