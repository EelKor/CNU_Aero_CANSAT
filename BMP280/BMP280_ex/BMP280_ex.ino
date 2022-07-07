#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>




#define SCL 8
#define SDA 9
#define CSB 10
#define SDO 11

Adafruit_BMP280 bmp(CSB, SDA, SDO, SCL);

void setup() {
  Serial.begin(9600);

  if (!bmp.begin()) {
    Serial.println(F("센서가 인식되지 않습니다. 연결 상태를 확인해주세요."));
    while (1);
  }
}

void loop() {
  delay(100);
  Serial.println("==========================");

  Serial.print(F("온도 =")); 
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("기압 ="));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("고도 ="));
  Serial.print(bmp.readAltitude(1006));
  Serial.println(" m");

  Serial.println("==========================");
  delay(10);
}
