#include<Wire.h>

const int MPU_ADDR = 0x68;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ; 
double angleAcX, angleAcY, angleAcZ;
double angleGyX, angleGyY, angleGyZ;

const double RADIAN_TO_DEGREE = 180 / 3.14159;  
const double DEG_PER_SEC = 32767 / 250; 


unsigned long now = 0;
unsigned long past = 0;  
double dt = 0;           

double averAcX, averAcY, averAcZ;
double averGyX, averGyY, averGyZ;

void setup() {
  initSensor();
  Serial.begin(9600);
  caliSensor(); 
  past = millis(); 
}

void loop() {
  getData(); 
  getDT();
  angleGyX += ((GyX - averGyX) / DEG_PER_SEC) * dt;
  angleGyY += ((GyY - averGyY) / DEG_PER_SEC) * dt;
  angleGyZ += ((GyZ - averGyZ) / DEG_PER_SEC) * dt;
  
  Serial.print("Angle Gyro X:");
  Serial.print(angleGyX);
  Serial.print("\t\t Angle Gyro y:");
  Serial.print(angleGyY);  
  Serial.print("\t\t Angle Gyro Z:");
  Serial.println(angleGyZ);  
  delay(20);
}

void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);  
  Wire.write(0x6B);  
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true); 
  AcX = Wire.read() << 8 | Wire.read(); 
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

void getDT() {
  now = millis();   
  dt = (now - past) / 1000.0;  
  past = now;
}

void caliSensor() {
  double sumAcX = 0 , sumAcY = 0, sumAcZ = 0;
  double sumGyX = 0 , sumGyY = 0, sumGyZ = 0;
  getData();
  for (int i=0;i<10;i++) {
    getData();
    sumAcX+=AcX;  sumAcY+=AcY;  sumAcZ+=AcZ;
    sumGyX+=GyX;  sumGyY+=GyY;  sumGyZ+=GyZ;
    delay(50);
  }
  averAcX=sumAcX/10;  averAcY=sumAcY/10;  averAcZ=sumAcY/10;
  averGyX=sumGyX/10;  averGyY=sumGyY/10;  averGyZ=sumGyZ/10;
}
