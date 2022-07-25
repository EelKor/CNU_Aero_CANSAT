f=open("TestData.txt",'w')

import time
import serial

RXport = "COM5"

arduino = serial.Serial(port = RXport, baudrate = 9600)
time.sleep(3)

arduino.write(b"test string")
time.sleep(1)

while(True):
    data = arduino.read_all()
    f.write(data)


f.close()
