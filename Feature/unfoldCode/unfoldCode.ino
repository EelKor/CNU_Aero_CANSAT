//bmp
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);
float tem ;//온도
float pa; //압력
float high ; //고도
float setHigh;

#include <Servo.h>
Servo servo;
int value = 1;
int unfoldValue = 90;
String data;
float unfoldHigh = 10; //낙하산 전개 고도
float prepareHigh = 5;
bool isPrepare = 0;

#define DEBUG


void setup() {
  Serial.begin(115200);

   //bmp
 if (!bmp.begin()) {
    Serial.println(F("센서가 인식되지 않습니다. 연결 상태를 확인해주세요."));
    while (1);
  } // */
  Serial.println("bmp ok");
  setHigh =  bmp.readAltitude(1006);

     //servo
  servo.attach(7);
  value = 0;
  servo.write(value);
  delay(100);
  Serial.println("servo ok");
  servo.detach();
  delay(100);
}

void loop() {
  high = bmp.readAltitude(1006) - setHigh;
  if(!isPrepare && high<prepareHigh){
    isPrepare = 0;
  }
  else if (!isPrepare && high>=prepareHigh){
    isPrepare = 1;
  }
  else if (isPrepare && high<unfoldHigh){
    servo.attach(7);
    servo.write(unfoldValue);
    delay(100);
    servo.detach();
  }
 #ifdef DEBUG
 Serial.println(String(high)+' '+String(isPrepare)+' '+String(servo.read()));
 #endif
}
