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

  
  
  출력결과

                                     각        각속도       가속도
  주기  낙하속도 온도  기압  고도  --X  Y  Z--    X  Y  Z    X  Y  Z   위도  경도
  */

//bmp id 0x58
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
#define SAMPLE_PERIOD (0.01f)
    #define AHRS true
    #define SerialDebug true
    MPU9250 myIMU;
    FusionAhrs ahrs;
#define MPU9250_I2C_ADDRESS 0x68

//bmp
#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);
float tem ;//온도
float pa; //압력
float high ; //고도
float setHigh;

//lora
//SoftwareSerial lora(2,3);

//time & fall speed
int t, dt=0, dt1 = 0, dt2 = 0;
float dH=0, dH1=0, dH2 = 0;
float FS = 0, FS1 = 0, FS2 = 0;

//gps
long lat,lon;
SoftwareSerial gpsSerial(5,6);
TinyGPS gps;

void setup() {
  Serial.begin(9600);
  //lora
 /* lora.begin(9600);
  Serial.println("lora setup");
  Serial.println("AT+IPR = 9600"); //로라 속도
  Serial.println("AT+ADDRESS = 70"); //로라 주소 지정
  Serial.println("AT+NETWORKID = 70"); //네트워크 아이디
  Serial.println("lora setup end");*/
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

//gyro
  Wire.begin();
  byte c = myIMU.readByte(MPU9250_ADDRESS_AD0, WHO_AM_I_MPU9250);
  
  if (c == 0x71)
  {
    myIMU.MPU9250SelfTest(myIMU.selfTest);
    myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
    myIMU.initMPU9250();
    byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    myIMU.initAK8963(myIMU.factoryMagCalibration);
    myIMU.getAres();
    myIMU.getGres();
    myIMU.getMres();
    myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
    FusionAhrsInitialise(&ahrs);
  }
    Serial.println("gyro ok");
    
//통신 셋업
 /* lora.println(F("                     각       각속도       가속도"));
  lora.println(F("주기  온도  기압  고도 X  Y  Z    X  Y  Z    X  Y  Z   위도  경도"));
  while(lora.available()){
    Serial.write(lora.read()); //전송
  }*/
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

//gyro
  myIMU.readAccelData(myIMU.accelCount);
  myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes;
  myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes;
  myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes;
  myIMU.readGyroData(myIMU.gyroCount);
  myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
  myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
  myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;
  myIMU.readMagData(myIMU.magCount);
  myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
                 * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
  myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
                 * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
  myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
                 * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];

  const FusionVector accelerometer = {myIMU.ax, myIMU.ay, myIMU.az}; 
  const FusionVector gyroscope = {myIMU.gx, myIMU.gy, myIMU.gz};

  FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, SAMPLE_PERIOD);
  const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));

//평균 낙하속도
 float aveFS = (FS+FS1+FS2)/3;

//전송코드
 String potval = /*String(dt)+' '+String(dt1)+' '+*/String(dt2)+' '+String(aveFS)+"    "+String(tem)+' '+String(pa)+' '+String(high)+"    "
                  +String(euler.angle.roll)+' '+String(euler.angle.pitch)+' '+String(euler.angle.yaw)
                  +"    "+String(myIMU.gx)+' '+String(myIMU.gy)+' '+String(myIMU.gz)+"    "+String(myIMU.ax)+' '+String(myIMU.ay)+' '+String(myIMU.az)
                  +"    "+String(lat)+' '+String(lon);//전송내용 문자열로 변환
 /*  String cmd = "AT+SEND= 70,"+String(potval.length()) +','+ String(potval)+"\r"; //전송코드

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
  }*/
  Serial.println(potval);
 
  
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

//모듈로 낙하산 전개 코드
void unfold (){
bool unfoldHigh (high > 200);
if (unfoldHigh < 200){

 }
}
