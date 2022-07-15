/*
 송신부 통신 및 자이로 담당 아두이노 코드
 
  mpu9250
  vcc 5v
  sda a4    scl a5

  Lora_trans
  VCC 3.3V 
  tx 2      rx 3 
  
  주기 낙하속도 온도 기압 고도 위도 경도 각(x y z) 각속도(x y z) 가속도(x y z)
  */

#include <SoftwareSerial.h>

//아두이노 시리얼 통신
SoftwareSerial mySerial(A2,A3);
  String sensorData;

//gyro
#include "MPU9250.h"
#include "Fusion.h"
#include <stdbool.h>
#include <stdio.h>
#define MPU9250_I2C_ADDRESS 0x68
#define SAMPLE_PERIOD (0.01f)
    #define AHRS true
    #define SerialDebug true
    MPU9250 myIMU;
    FusionAhrs ahrs;


//lora
SoftwareSerial lora(2,3);

//time
int t, dt=0;


void setup() {
   Serial.begin(9600);
  //lora
  lora.begin(9600);
  Serial.println("lora setup");
  Serial.println("AT+IPR = 9600"); //로라 속도
  Serial.println("AT+ADDRESS = 70"); //로라 주소 지정
  Serial.println("AT+NETWORKID = 70"); //네트워크 아이디
  Serial.println("lora setup end");

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
}

void loop() {
 t = millis();
  
 //gyro
  myIMU.readAccelData(myIMU.accelCount);
  myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes;
  myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes;
  myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes;
  myIMU.readGyroData(myIMU.gyroCount);
  myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
  myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
  myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;
 /* myIMU.readMagData(myIMU.magCount);
  myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
                 * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
  myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
                 * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
  myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
                 * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];*/

  const FusionVector accelerometer = {myIMU.ax, myIMU.ay, myIMU.az}; 
  const FusionVector gyroscope = {myIMU.gx, myIMU.gy, myIMU.gz};

  FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, SAMPLE_PERIOD);
  const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));


//시리얼 수신

  while(mySerial.available()){
    if(mySerial.available()){
    sensorData = String(mySerial.readString()); //전송받은 문자열
    }
  }


//전송코드
 String potval =String(dt)+' '+String(sensorData) +"    "+ String(euler.angle.roll)+' '+String(euler.angle.pitch)+' '+String(euler.angle.yaw)
                  +"    "+String(myIMU.gx)+' '+String(myIMU.gy)+' '+String(myIMU.gz)+
                  "    "+String(myIMU.ax)+' '+String(myIMU.ay)+' '+String(myIMU.az);//전송내용 문자열로 변환
                  
  String cmd = "AT+SEND= 70,"+String(potval.length()) +','+ String(potval)+"\r"; //전송코드

  String inString;//받은 문자열

  lora.println(cmd);
  while(lora.available()){
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
}

//자이로 각이 급속도로 변하면 보조낙하산 전개
