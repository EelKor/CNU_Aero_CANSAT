/* bmp280:기압/온도/고도
  vcc 3.3v  SCL 8
  SDA 9     SCB 10
  SDO 11
  
  gps -n8m
  vcc 5V
  tx  2  rx  3

  mpu6050
  vcc 5v
  sda a4    scl a5
  int 2

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
SoftwareSerial GPS(3,4);  
byte buff[100];
  String lat; double lat_dd;
  String lng; double lng_dd;

//gyro
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define INTERRUPT_PIN 2

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


//time & fall speed
unsigned int t, dt=0;
float dH=0, prvHigh = 0;
float FS = 0;


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

//라즈베리파이 시리얼 통신
String cmd;


/*======================================*/

void setup(){
      Serial.begin(9600);

//gps
 GPS.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Goodnight moon!");


  
//라즈베리파이 시리얼 통신
    //  dataSerial.begin(9600);
    
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

/*===========================================*/

void loop()
{
    t = millis();

//gps
 if (GPS.available()) {
    GPS.readBytesUntil('\n',buff,100);
    String msg = buff;
    if(msg.substring(0,6).equals("$GNGGA") || msg.substring(0,6).equals("$GPGGA"))
    {
     
     int index = msg.indexOf(",");// 첫 번째 콤마 위치
     int index2= msg.indexOf(",",index+1); 
     int index3 = msg.indexOf(",",index2+1); 
     int index4 = msg.indexOf(",",index3+1); 
     int index5 = msg.indexOf(",",index4+1);
     int index6 = msg.indexOf(",",index5+1);  
     String positionFix = msg.substring(index6+1,index6+2);
      if (index6<43){positionFix = "0";}
     //Serial.println(positionFix);
     
     if(positionFix != "0"){   
      lat = msg.substring(17,29);
      lng = msg.substring(30,43);
      
    /* int d_index_lat = lat.indexOf(".");
     int d_index_lng = lng.indexOf(".");
     
     String d_lat = lat.substring(0, d_index_lat-2);
     String d_lng = lng.substring(0, d_index_lng-2);

     String m_lat = lat.substring(d_index_lat-2,lat.length()-2);
     String m_lng = lng.substring(d_index_lng-2,lng.length()-2);
     
     double d_lat_double = d_lat.toDouble();
     double d_lng_double = d_lng.toDouble();

     double m_lat_double = m_lat.toDouble();
     double m_lng_double = m_lng.toDouble();

     lat_dd = d_lat_int + m_lat_double/60;
     lng_dd = d_lng_int + m_lng_double/60;*/
     }

     else{
     // lat_dd = 0; lng_dd = 0;
      lat = '0'; lng = '0';
    }
 }
 /* else{//lat_dd = 377; lng_dd = 377;
       lat = "377"; lng = "377";}*/
      
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


//time & fall speed
    dt = millis()-t;
    high = bmp.readAltitude(1006) - setHigh; //고도 */
    dH =high - prvHigh;
    FS= dH/dt;



  prvHigh = bmp.readAltitude(1006) - setHigh;
   cmd = String(dt)+','+String(FS)+','+String(tem)+','+String(pa)+','+String(high)+','
       +String(ypr[0] * 180/M_PI)+','+String(ypr[1] * 180/M_PI)+','+String(ypr[2] * 180/M_PI)
       +','/*+String(lat_dd)+','+String(lng_dd)+','*/+String(lat)+','+String(lng);//전송내용 문자열로 변환;
  Serial.println(cmd);

  unfold();

}
}
}
