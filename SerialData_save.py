f=open("cansatData.txt",'w')
f.close

import datetime
import serial

arduino = serial.Serial(port = "COM10", baudrate = 9600)

while True:
    if arduino.readable():
        now = datetime.datetime.now()
        f=open("cansatData.txt",'a',encoding = "UTF8")
        
        #cmdString = input()
        #if len(cmdString)>0:
            #if cmdString == "end":
                #f.close()
                 #break
            #cmdStringByte = cmdString.encode('utf-8')
            #arduino.write(cmdStringByte)
            #f.write(cmdString)
    
        dataString = arduino.readline()
        dataString_decoded = dataString.decode()[:len(dataString)-1]
        print(now.strftime('%Y-%m-%d %H:%M:%S '))
        print(dataString_decoded)
        f.write(now.strftime('%Y-%m-%d %H:%M:%S '))
        f.write(dataString_decoded)
        f.close()
