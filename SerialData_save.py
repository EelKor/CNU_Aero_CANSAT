import datetime
import serial
now = datetime.datetime.now()

arduino = serial.Serial(port = "COM7", baudrate = 9600)

while True:
    if arduino.readable():
        f=open("TestData.txt",'a',encoding = "UTF8")
        #cmdString = input()
        #if cmdString == "end":
            #f.close()
            #break
        #cmdStringByte = cmdString.encode('utf-8')
        #arduino.write(cmdStringByte)
        #f.write(cmdString)
        
        dataString = arduino.readline()
        dataString_decoded = dataString.decode()[:len(dataString)-1]
        print(dataString_decoded)
        f.write(now.strftime('%Y-%m-%d %H:%M:%S \t'))
        f.write(dataString_decoded)
        f.close()
