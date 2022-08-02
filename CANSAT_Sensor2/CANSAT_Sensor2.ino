/* 
  bmp280:기압/온도/고도
  vcc 3.3v  SCL 8
  SDA 9     SCB 10
  SDO 11
  
  gps -n8m
  vcc 5V
  tx 3  rx  4

  mpu6050
  vcc 5v
  sda a4    scl a5
  int 2

  servo
  pwm 7 

  주기 낙하속도 온도 기압 고도 x y z 위도 경도
*/

/**************** 디버그 모드 ***********************/
#define UNFOLD
//#define DEBUG

/***************************************************/


// gps
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);
String latData, lngData;

void gpsData()
{
  if (gps.location.isValid())
  {
   /* Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);*/
    latData = String(gps.location.lat(),6);
    lngData = String(gps.location.lng(),6);
  }
  else
  {
    latData = "null";
    lngData = "null";
  }
}

float gpsAltitude;

// bmp id 0x58
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

// gyro
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define INTERRUPT_PIN 2

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL
#define OUTPUT_READABLE_REALACCEL

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
int16_t gx, gy, gz;


volatile bool mpuInterrupt = false;
void dmpDataReady() 
{
    mpuInterrupt = true;
}

// bmp
#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);
const float stdPa = 1013;
float tem;//온도
float pa; //압력
float high; //고도
float setHigh;
unsigned long t, dt=0;
float dH=0, prvHigh = 0;
float FS = 0;
int fallStack = 0;


// servo
#include <Servo.h>
Servo servo;
int Value = 0;
int unfoldValue = 90;
String data;
float unfoldHigh = 7; // 낙하산 전개 고도
float prepareHigh =10;
bool isUnfolded = 0;

#ifdef UNFOLD
bool isPrepare = 0;
void unfold()
{
  if(!isPrepare && high<prepareHigh && !isUnfolded )
  {
    isPrepare = 0;
  }
  else if (!isPrepare && high>=prepareHigh && !isUnfolded)
  {
    isPrepare = 1;
  }
  else if (isPrepare && high<unfoldHigh && !isUnfolded)
  {
    if (dH < 0) fallStack++;
    else  fallStack--;

    if(fallStack >15)
    {
      servo.attach(7);
      servo.writeMicroseconds(1500);
      delay(1000);
      servo.detach();
      isUnfolded = 1;
    }
  }
}
#else

// Unfold 디버그 코드 - #define UNFOLD 가 선언되지 않았으면
void unfold()
{
  if(!isUnfolded && high>=7)
  {
    servo.attach(7);
    servo.writeMicroseconds(1500);
    delay(1000);
    servo.detach();
    isUnfolded = 1;

  }
  else
  {
    #ifdef DEBUG
    while(1) Serial.println("unfold() - BUG");
    #endif
  }
}
#endif

//라즈베리파이 시리얼 통신
String cmd;

/*======================================*/

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
  
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  
  // gyro
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000);
    
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

  while (!Serial);

  devStatus = mpu.dmpInitialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788);

  if (devStatus == 0) 
  {
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
  Serial.println("gyro ok");

// BMP 센서 초기화 및 최초 고도 설정
  if (!bmp.begin()) 
  {
    Serial.println(F("BMP - FAIL"));
    while (1);
  }

  setHigh = 0;
  for(int i = 0; i < 10; i++)
  {
    setHigh += bmp.readAltitude(stdPa);
  }
  setHigh = setHigh / 10;
  Serial.println("BMP - OK");
}
/*===========================================*/

void loop()
{
  t = millis();
    
// GPS 센서데이터 읽기
  while (ss.available() > 0) if (gps.encode(ss.read())) gpsData();
    
    
// BMP 센서 - 온도, 압력, 고도 읽기
  tem = bmp.readTemperature();  // 온도
  pa = bmp.readPressure();      // 압력
  high = bmp.readAltitude(stdPa) - setHigh;

// IMU 센서 - YPR 각도 읽기
  if (!dmpReady) return;
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) 
  {
    mpu.resetFIFO();    
  } 
    
  else if (mpuIntStatus & 0x02) 
  {
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

    mpu.getRotation(&gx,&gy,&gz);
  }


// time & fall speed
  dt = millis()-t;
  float sumHigh=0;
  for(int i=0; i<3; i++)
  {
    sumHigh += bmp.readAltitude(stdPa) - setHigh; //고도 */
    delay(10);
  }
  high = sumHigh/3;
  dH =high - prvHigh;


  FS= dH/dt;
  prvHigh = high;
   while (ss.available() > 0) if (gps.encode(ss.read())) gpsAltitude = gps.altitude.meters();


  cmd =String(dt)+','+String(aaReal.x)+','+String(aaReal.y)+','+String(aaReal.z)+','+String(gx)+','+String(gy)+','+String(gz)
       +','+String(pa)+','+String(high)+','+String(gpsAltitude)+','+String(tem)+','+String(FS)+','+String(ypr[1] * 180/M_PI)+','+String(ypr[2] * 180/M_PI)
       +','+latData + ',' + lngData; //전송내용 문자열로 변환;
  Serial.println(cmd);

  unfold();

}
