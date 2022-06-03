# REYAX RYLR896 LoRa 통신 모듈
## 1. 핀 레이아웃
<https://how2electronics.com/reyax-rylr890-lora-module-with-arduino/>

- 해당 모듈은 3.3V로 구동되므로 VDD 에 3.3v 인가
- 아두이노의 디지털핀은 5V 전압을 출력하므로, 4.6K, 10K 저항을 이용해 전압 강하회로 구성(링크참조)

## 2. SoftwareSerial 사용시 주의사항
 - 해당모듈은 Arduino와 시리얼 통신으로 작동됨, 이때 Arduino Uno 같은 경우 HardwareSerial를 컴퓨터와 통신하는데 사용하므로, SoftwareSerial 사용이 필수적임.
 - RYLR896 기본 Baud rate = 115200 bps, 하지만 Arduino Uno 의 SoftwareSerial 에서 115200 bps 의 속도는 매우 불안정함. 따라서 **9600 bps** 로 변경하여 사용
```
AT+IPR=<rate>
<rate> is the UART baud rate：
300
1200
4800
9600
19200
28800
38400
57600
115200(default)
Example: Set the baud rate as 9600,
*The settings will be memorized in EEPROM.
AT+IPR=9600
```
```
AT+IPR=9600
```

## 3. 기타설정
- RYLR986의 기본 NETWORKID 는 공용ID인 0 이므로, 다른 ID로 변경하여 사용하는것이 좋다.
```
AT+NETWORKID=<Network ID>
<Network ID>0~255 (default 0)

Example: Set the network ID as 6,
*The settings will be memorized in EEPROM.
*The ”0” is the public ID of Lora. It is not recommended to set 0 to make the distinction of NETWORK.
```
```
AT+NETWORKID=6
```

 - Address 또한 충돌가능성으로 기본값 0 에서 변경하여 사용하는것이 좋다.
 ```
 AT+ADDRESS=<Address>
<Address>=0~65535 (default 0)

Example: Set the address of module as 120.
*The settings will be memorized in EEPROM.
```
```
AT+ADDRESS=120
```



