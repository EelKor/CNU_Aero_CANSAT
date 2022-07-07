/* Madgwick 알고리즘과 IMU9250 을 이용한 AHRS */

/* 핀 레이아웃
 * Arduino - IMU9250  
      5V - VCC  
      GND - GND  
      SCL - A5  
      SDA - A4
*/


 #include "Fusion.h"
 #include <stdbool.h>
 #include <stdio.h>
 #define SAMPLE_PERIOD (0.01f) // replace this with actual sample period
 FusionAhrs ahrs;
 
 #include "MPU9250.h"
 #define AHRS true         // Set to false for basic data read
 #define SerialDebug true  // Set to true to get Serial output for debugging
 MPU9250 myIMU;


void setup() 
{
  Wire.begin();
  //I2C통신 준비
  // TWBR = 12;  // 400 kbit/sec I2C speed
  Serial.begin(115200);
  //38400 bits/s
  // Read the WHO_AM_I register, this is a good test of communication
  byte c = myIMU.readByte(MPU9250_ADDRESS_AD0, WHO_AM_I_MPU9250);
  
  if (c == 0x71) // WHO_AM_I should always be 0x71 
  {
	  // Calibrate gyro and accelerometers, load biases in bias registers
	  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
	  myIMU.initMPU9250();
	  // Initialize device for active mode read of acclerometer, gyroscope, and
	  // temperature
	  // Read the WHO_AM_I register of the magnetometer, this is a good test of
	  // communication
	  byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
	  // Get magnetometer calibration from AK8963 ROM
	  myIMU.initAK8963(myIMU.factoryMagCalibration);
	  // Initialize device for active mode read of magnetometer
	  // Get sensor resolutions, only need to do this once
	  myIMU.getAres();
	  myIMU.getGres();
	  myIMU.getMres();
	  // The next call delays for 4 seconds, and then records about 15 seconds of
	  // data to calculate bias and scale.
	  myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
	  delay(2000);
	  // Add delay to see results before serial spew of data
  }

  else 
  {
	  Serial.print("Could not connect to MPU9250: 0x");
	  Serial.println(c, HEX);
	  while(1) ;
	  // Loop forever if communication doesn't happen
  }

  FusionAhrsInitialise(&ahrs);
}

void loop() 
{
  myIMU.readAccelData(myIMU.accelCount);
  // Read the x/y/z adc values
  // Now we'll calculate the accleration value into actual g's
  // This depends on scale being set
  myIMU.ax = (float)myIMU.accelCount[0] * myIMU.aRes;
  // - myIMU.accelBias[0];
  myIMU.ay = (float)myIMU.accelCount[1] * myIMU.aRes;
  // - myIMU.accelBias[1];
  myIMU.az = (float)myIMU.accelCount[2] * myIMU.aRes;
  // - myIMU.accelBias[2];
  myIMU.readGyroData(myIMU.gyroCount);
  // Read the x/y/z adc values
  // Calculate the gyro value into actual degrees per second
  // This depends on scale being set
  myIMU.gx = (float)myIMU.gyroCount[0] * myIMU.gRes;
  myIMU.gy = (float)myIMU.gyroCount[1] * myIMU.gRes;
  myIMU.gz = (float)myIMU.gyroCount[2] * myIMU.gRes;
  myIMU.readMagData(myIMU.magCount);
  // Read the x/y/z adc values
  // Calculate the magnetometer values in milliGauss
  // Include factory calibration per data sheet and user environmental
  // corrections
  // Get actual magnetometer value, this depends on scale being set
  myIMU.mx = (float)myIMU.magCount[0] * myIMU.mRes
                 * myIMU.factoryMagCalibration[0] - myIMU.magBias[0];
  myIMU.my = (float)myIMU.magCount[1] * myIMU.mRes
                 * myIMU.factoryMagCalibration[1] - myIMU.magBias[1];
  myIMU.mz = (float)myIMU.magCount[2] * myIMU.mRes
                 * myIMU.factoryMagCalibration[2] - myIMU.magBias[2];
  
  const FusionVector accelerometer = {myIMU.ax, myIMU.ay, myIMU.az}; // replace this with actual gyroscope data in degrees/s
  const FusionVector gyroscope = {myIMU.gx, myIMU.gy, myIMU.gz}; // replace this with actual accelerometer data in g
  FusionAhrsUpdateNoMagnetometer(&ahrs, gyroscope, accelerometer, SAMPLE_PERIOD);
  const FusionEuler euler = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&ahrs));

  Serial.write(2);
  Serial.print(euler.angle.roll);
  Serial.print(" ");
  Serial.print(euler.angle.pitch);
  Serial.print(" ");
  Serial.println(euler.angle.yaw);
  

  // Must be called before updating quaternions!
  myIMU.updateTime();
}