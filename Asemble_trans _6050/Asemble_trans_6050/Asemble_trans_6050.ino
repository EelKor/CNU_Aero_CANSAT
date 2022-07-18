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

// GPS
#include <TinyGPS.h>

//gyro
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif
#define INTERRUPT_PIN 2

MPU6050 mpu;

#define OUTPUT_READABLE_YAWPITCHROLL

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
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
float prevHigh;

//lora
//SoftwareSerial lora(2,3);

//time & fall speed
unsigned long dt, startTime;
float FS;


//gps
long lat,lon;
SoftwareSerial gpsSerial(5,6);
TinyGPS gps;

void setup() {
  Serial.begin(115200);
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
 #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
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
    
//통신 셋업
 /* lora.println(F("                     각       각속도       가속도"));
  lora.println(F("주기  온도  기압  고도 X  Y  Z    X  Y  Z    X  Y  Z   위도  경도"));
  while(lora.available()){
    Serial.write(lora.read()); //전송
  }*/
}


void loop() {
  startTime = millis();

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
if (!dmpReady) return;
mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetEuler(euler, &q);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
        #endif
    
        }

 
//평균 낙하속도
dt = millis() - startTime;
FS = (high - prevHigh) / dt;

//전송코드
 String potval = /*String(dt)+' '+String(dt1)+' '+*/String(dt)+' '+String(FS)+"    "+String(tem)+' '+String(pa)+' '+String(high)+"    "
                 /* +String(q.w)+' '+String(q.x)+' '+String(q.y)+' '+String(q.z)+' '
                  +String(euler[0] * 180/M_PI)+' '+String(euler[1] * 180/M_PI)+' '+String(euler[2] * 180/M_PI)+' '*/
                  +String(ypr[0] * 180/M_PI)+' '+String(ypr[1] * 180/M_PI)+' '+String(ypr[2] * 180/M_PI)+' '
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
  prevHigh = high;

}
