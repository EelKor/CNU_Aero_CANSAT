/* 
 송신부 센서(자이로 제외) 코드 
  
  bmp280:기압/온도/고도
  vcc 3.3v  SCL 8
  SDA 9     SCB 10
  SDO 11

  gps -6m 기준
  vcc 5V
  tx  5   rx  6


  */

#include <SoftwareSerial.h>

//아두이노 시리얼 통신
SoftwareSerial mySerial(A2,A3);

//bmp id 0x58
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

//gps
#include <TinyGPS.h>
long lat,lon;
SoftwareSerial gpsSerial(5,6);
TinyGPS gps;

//time & fall speed
int t, dt=0, dt1 = 0, dt2 = 0;
float dH=0, dH1=0, dH2 = 0;
float FS = 0, FS1 = 0, FS2 = 0;


void setup() {
Serial.begin(9600);
mySerial.begin(9600);

//bmp
 if (!bmp.begin()) {
    Serial.println(F("센서가 인식되지 않습니다. 연결 상태를 확인해주세요."));
    while (1);
  } // */
  Serial.println("bmp ok");
  setHigh =  bmp.readAltitude(1006);

  //gps
  gpsSerial.begin(9600);
    Serial.println("gps ok");


}

void loop() {
 t = millis();
  
//bmp

  tem = bmp.readTemperature();//온도
  pa = bmp.readPressure(); //압력
  high = bmp.readAltitude(1006) - setHigh; //고도 */
  
//gps
  while(gpsSerial.available()){ 
  if(gps.encode(gpsSerial.read())){
   gps.get_position(&lat,&lon);
  }
  }

  
//평균 낙하속도
 float aveFS = (FS+FS1+FS2)/3;
 String sensorStr;
String sensorData = String(dt2)+' '+String(aveFS)+"    "+
                    String(tem)+' '+String(pa)+' '+String(high)+"    "+
                    String(lat)+' '+String(lon);
mySerial.println(sensorData);
  while(mySerial.available()){
    if(mySerial.available()){
    sensorStr = String(mySerial.readString()); //전송받은 문자열
    }
  }
   if(sensorStr.length()>0)
  {
    Serial.println(sensorStr); //문자열 출력
  }
Serial.println(sensorData);


 //time & fall speed
  dt = dt1;
  dt1 = dt2;
  dt2 = millis()-t;

  dH = dH1;
  dH1 = dH2;
  dH2 = high - (bmp.readAltitude(1006) - setHigh);

  FS = FS1;
  FS1 = FS2;
  FS2 = dH2/dt2;
}

//high가 어느 정도 이하로 떨어지면 주 낙하산 전개
