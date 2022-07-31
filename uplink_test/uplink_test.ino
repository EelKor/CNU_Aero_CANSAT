#define V1_BOARD
//#define V2_BOARD
#define LORA

//bmp id 0x58
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>


#include <SoftwareSerial.h>
//gps
#ifdef V1_BOARD
SoftwareSerial GPS(2,3);
#endif
#ifdef V2_BOARD
SoftwareSerial GPS(3,4);
#endif  
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
unsigned long t, dt=0;
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

//lora
#ifdef LORA
SoftwareSerial lora(6,5);
#endif


void setup() {
  Serial.begin(9600);

  //gps
 GPS.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Goodnight moon!");

 //lora
  #ifdef LORA
  lora.begin(9600);
  lora.println("AT+PARAMETER=10,7,1,7");
  delay(100);
  lora.println("AT+IPR=9600"); //로라 속도
  delay(100);
  lora.println("AT+ADDRESS=75"); //로라 주소 지정
  delay(100);
  lora.println("AT+NETWORKID=2"); //네트워크 아이디
  delay(100);
  Serial.println("lora setup end");
  #endif
}

void loop() {
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
 }
       
#ifdef LORA
  String inString;
   while(lora.available())
  {
    if(lora.available())
    {
     inString = String(lora.readStringUntil('\n'));
     //lora.flush();
    }
  }

  if(inString.length() > 0)
  {
    Serial.println(inString);
  }
#endif
}
