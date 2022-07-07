//송신부 코드입니다.

/* bmp280:기압/온도/고도
  vcc 3.3v  SCL 8
  SDA 9     SCB 10
  SDO 11
  
  Lora_trans
  VCC 3.3V 
  tx 2      rx 3 
  
  gps -6m 기준
  vcc 5V
  tx  5   rx  6

  mpu9250
  vcc 5v
  sda a4    scl a5
  */


#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>

//gyro
#include "MPU9250.h"
#include "Fusion.h"
#include <stdbool.h>
#include <stdio.h>
#define SAMPLE_PERIOD (0.01f) // replace this with actual sample period

    #define AHRS true         // Set to false for basic data read
    #define SerialDebug true  // Set to true to get Serial output for debugging
    MPU9250 myIMU;

    FusionAhrs ahrs;


//bmp
#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);
SoftwareSerial lora(2,3);

float t;
float dt=0;

//gps
long lat,lon;
SoftwareSerial gpsSerial(5,6);
TinyGPS gps;

void setup() {
  Serial.begin(9600);
  //lora
  lora.begin(9600);
  Serial.println("lora setup");
  Serial.println("AT+IPR = 9600"); //로라 속도
  Serial.println("AT+ADDRESS = 70"); //로라 주소 지정
  Serial.println("AT+NETWORKID = 70"); //네트워크 아이디
  Serial.println("lora setup end");
  lora.println("lora setup end");
    while(lora.available()){
    Serial.write(lora.read()); //전송
  }
  //bmp
  if (!bmp.begin()) {
    Serial.println(F("bmp error")); //bmp에러 확인
    lora.println("bmp error");
    while(lora.available()){
    Serial.write(lora.read()); //전송
  }

  //gps
  Serial.println("Start GPS... ");
  gpsSerial.begin(9600);
  }


  Serial.println("                              각       각속도       가속도                  ");
  Serial.println("시간   온도   기압   고도     X  Y  Z    X  Y  Z    X  Y  Z   지자기    위도     경도");
  Serial.println("---------------------------------------------------------------------------------");
  
  lora.println("                              각       각속도       가속도                  ");
  lora.println("시간   온도   기압   고도     X  Y  Z    X  Y  Z    X  Y  Z   지자기    위도     경도");
  lora.println("---------------------------------------------------------------------------------");
  while(lora.available()){
    Serial.write(lora.read()); //전송
  }
}

void loop() {
  t = millis();

  float tem = Serial.print(bmp.readTemperature()); //온도
  float pa = Serial.print(bmp.readPressure()); //압력
  float high = Serial.print(bmp.readAltitude(1006)); //고도

  while(gpsSerial.available()){ 
  if(gps.encode(gpsSerial.read())){
   gps.get_position(&lat,&lon);
  }

  String potval = String(t)+"  "+String(tem)+"  "+String(pa)+"  "+String(high);//전송내용 문자열로 변환
  String cmd = "AT+SEND= 70,"+String(potval.length()) +","+ String(potval)+"\r"; //전송코드

  String inString;//받은 문자열

  lora.println(cmd);
  while(lora.available()){
    Serial.write(lora.read());
    if(lora.available()){
    inString = String(char(lora.read())); //전송받은 문자열
    }
  }
   if(inString.length()>0)
  {
    Serial.println(inString); //문자열 출력
  }
  Serial.println(potval);
  dt = millis()-t;
  //시리얼 모니터 모니터링 편하게 수정하기 : 간격&레이아웃
  }
}
