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

  mpu6050
  vcc 5v
  sda a4    scl a5
  int 13

  servo
  pwm 7

  주기 낙하속도 온도 기압 고도 x y z 위도 경도
  */

//bmp id 0x58
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>


#include <SoftwareSerial.h>
//gps
#include <TinyGPS.h>

//gyro
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define INTERRUPT_PIN 13

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

 bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;
VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 aaWorld;
VectorFloat gravity;
float euler[3];
float ypr[3]; 

uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

volatile bool mpuInterrupt = false;
void dmpDataReady() {
    mpuInterrupt = true;}

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
int t, dt=0;
float dH=0, prvHigh = 0;
float FS = 0;

//gps
long lat,lon;
SoftwareSerial gpsSerial(5,6);
TinyGPS gps;

//servo
#include <Servo.h>
Servo servo;
int value = 0;
int unfoldValue = 90;
String data;
float unfoldHigh = 300; //낙하산 전개 고도

void unfold(){
  if (high<= unfoldHigh){
    servo.write(unfoldValue);
  }
}




unsigned long lastTransmission;
const int interval = 1000;
SoftwareSerial lora(2,3);  // Lora TX , Lora RX

void setup(){
      Serial.begin(9600);
      
//lora
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
    
  //gyro
 #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000);
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);sa
    #endif
    while (!Serial);
     devStatus = mpu.dmpInitialize();
     mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

     if (devStatus == 0) {
        mpu.setDMPEnabled(true);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
    }
    Serial.println("gyro ok");

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
  Serial.println("servo ok");
  delay(100);

}

void loop()
{
    t = millis();

    //bmp

  tem = bmp.readTemperature();//온도
  pa = bmp.readPressure(); //압력
  

    //gyro
if (!dmpReady) return;
mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
      } 
    else if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            mpu.dmpGetQuaternion(&q, fifoBuffer);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            teapotPacket[2] = fifoBuffer[0];
            teapotPacket[3] = fifoBuffer[1];
            teapotPacket[4] = fifoBuffer[4];
            teapotPacket[5] = fifoBuffer[5];
            teapotPacket[6] = fifoBuffer[8];
            teapotPacket[7] = fifoBuffer[9];
            teapotPacket[8] = fifoBuffer[12];
            teapotPacket[9] = fifoBuffer[13];
            Serial.write(teapotPacket, 14);
            teapotPacket[11]++; 
        #endif}


    dt = millis()-t;
    high = bmp.readAltitude(1006) - setHigh; //고도 */
    dH =high - prvHigh;
    FS= dH/dt;


    //전송
  String cmd = String(dt)+' '+String(FS)+' '+String(tem)+' '+String(pa)+' '+String(high)+' '
                  +String(ypr[0] * 180/M_PI)+' '+String(ypr[1] * 180/M_PI)+' '+String(ypr[2] * 180/M_PI)
                  /*+' '+String(lat)+' '+String(lon)*/;//전송내용 문자열로 변환;
      lora.println("AT+SEND=77,"+String(cmd.length())+","+cmd);
      delay(50);

//time & fall speed
  prvHigh = bmp.readAltitude(1006) - setHigh;

   String inString;

  while(lora.available())
  {
    if(lora.available())
    {
      inString = String(lora.readStringUntil('\n'));
    }
  }

  if(inString.length() > 0)
  {
    Serial.println(inString);
  }
  
}
}
