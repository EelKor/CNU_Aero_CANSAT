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
  Serial.print("MPU9250 ");
  Serial.print("I AM ");
  Serial.print(c, HEX);
  Serial.print(" I should be ");
  Serial.println(0x71, HEX);
  
  if (c == 0x71) // WHO_AM_I should always be 0x71 
  {
	  Serial.println("MPU9250 is online...");
	  // Start by performing self test and reporting values
	  myIMU.MPU9250SelfTest(myIMU.selfTest);
	  Serial.print("x-axis self test: acceleration trim within : ");
	  Serial.print(myIMU.selfTest[0],1);
	  Serial.println("% of factory value");
	  Serial.print("y-axis self test: acceleration trim within : ");
	  Serial.print(myIMU.selfTest[1],1);
	  Serial.println("% of factory value");
	  Serial.print("z-axis self test: acceleration trim within : ");
	  Serial.print(myIMU.selfTest[2],1);
	  Serial.println("% of factory value");
	  Serial.print("x-axis self test: gyration trim within : ");
	  Serial.print(myIMU.selfTest[3],1);
	  Serial.println("% of factory value");
	  Serial.print("y-axis self test: gyration trim within : ");
	  Serial.print(myIMU.selfTest[4],1);
	  Serial.println("% of factory value");
	  Serial.print("z-axis self test: gyration trim within : ");
	  Serial.print(myIMU.selfTest[5],1);
	  Serial.println("% of factory value");
	  // Calibrate gyro and accelerometers, load biases in bias registers
	  myIMU.calibrateMPU9250(myIMU.gyroBias, myIMU.accelBias);
	  myIMU.initMPU9250();
	  // Initialize device for active mode read of acclerometer, gyroscope, and
	  // temperature
	  Serial.println("MPU9250 initialized for active data mode....");
	  // Read the WHO_AM_I register of the magnetometer, this is a good test of
	  // communication
	  byte d = myIMU.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
	  Serial.print("AK8963 ");
	  Serial.print("I AM ");
	  Serial.print(d, HEX);
	  Serial.print(" I should be ");
	  Serial.println(0x48, HEX);
	  // Get magnetometer calibration from AK8963 ROM
	  myIMU.initAK8963(myIMU.factoryMagCalibration);
	  // Initialize device for active mode read of magnetometer
	  Serial.println("AK8963 initialized for active data mode....");
	  // Get sensor resolutions, only need to do this once
	  myIMU.getAres();
	  myIMU.getGres();
	  myIMU.getMres();
	  // The next call delays for 4 seconds, and then records about 15 seconds of
	  // data to calculate bias and scale.
	  myIMU.magCalMPU9250(myIMU.magBias, myIMU.magScale);
	  Serial.println("AK8963 mag biases (mG)");
	  Serial.println(myIMU.magBias[0]);
	  Serial.println(myIMU.magBias[1]);
	  Serial.println(myIMU.magBias[2]);
	  Serial.println("AK8963 mag scale (mG)");
	  Serial.println(myIMU.magScale[0]);
	  Serial.println(myIMU.magScale[1]);
	  Serial.println(myIMU.magScale[2]);
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

  Serial.print(myIMU.ax);
  Serial.print("\t");
  Serial.print(myIMU.ay);
  Serial.print("\t");
  Serial.print(myIMU.az);
  Serial.print("\t");
  Serial.print(myIMU.gx);
  Serial.print("\t");
  Serial.print(myIMU.gy);
  Serial.print("\t");
  Serial.print(myIMU.gz); 
  Serial.print("\t");
  Serial.print(myIMU.mx);
  Serial.print("\t");
  Serial.print(myIMU.my);
  Serial.print("\t");
  Serial.println(myIMU.mz);


  // Must be called before updating quaternions!
  myIMU.updateTime();
}