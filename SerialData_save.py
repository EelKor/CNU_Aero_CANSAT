f=open("TestData.txt",'w',encoding = "UTF8")

import time
import serial


arduino = serial.Serial(port = "COM9", baudrate = 9600)

while True:
    if arduino.readable():
        cmdString = input()
        if cmdString == "end":
            f.close()
            break
        cmdStringByte = cmdString.encode('utf-8')
        arduino.write(cmdStringByte)
        f.write(cmdString)
        
        dataString = arduino.readline()
        dataString_decoded = dataString.decode()[:len(dataString)-1]
        print(dataString_decoded)
        f.write(dataString_decoded)
