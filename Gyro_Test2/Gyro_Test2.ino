//vcc = 5v, gnd = gnd, scl=A5, sda = A4

#include <mpu9250.h>
#include <Wire.h>

MPU9250 IMU (Wire , 0x68);     
void setup() {
  Serial.begin(9600);
  IMU.begin(); 
}

void loop() {
  IMU.readSensor();
  Serial.print("Accelerometer X axis: ");   
  Serial.print(IMU.getAccelX_mss(), 3); 
  Serial.print("               Accelerometer Y axis: ");
  Serial.print(IMU.getAccelY_mss(), 3);
  Serial.print("               Accelerometer Z axis: ");
  Serial.println(IMU.getAccelZ_mss(), 3);
  
  Serial.print("Gyroscope X axis(radians): ");
  Serial.print(IMU.getGyroX_rads(), 3);
  Serial.print("           Gyroscope Y axis(radians): ");
  Serial.print(IMU.getGyroY_rads(), 3);
  Serial.print("           Gyroscope Z axis(radians): ");
  Serial.println(IMU.getGyroZ_rads(), 3);
  
  Serial.print("Magnetometer X axis(MicroTesla): ");
  Serial.print(IMU.getMagX_uT(), 3);
  Serial.print("    Magnetometer Y axis(MicroTesla): ");
  Serial.print(IMU.getMagY_uT(), 3);
  Serial.print("    Magnetometer Z axis(MicroTesla): ");
  Serial.println(IMU.getMagZ_uT(), 3);
  
  Serial.print("Temperature: ");
  Serial.println(IMU.getTemperature_C(), 2);  
  
  //Serial.print("*********** Next buffer data *****************");
  //delay(100);
  
 //데이터 효율적 전송을 위해서는 전송 내용을 간단히 정리할 필요가 있어 보임..
}
