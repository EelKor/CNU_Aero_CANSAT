f=open("cansatData.txt",'w')
f.close

import datetime, serial, time

arduino = serial.Serial(port = "COM8", baudrate = 9600)
dt = 0
cmdString = ""

while True:
    if arduino.readable():
        now = datetime.datetime.now()
        f=open("cansatData.txt",'a',encoding = "UTF8")
    #TX
        t= time.time()
        if t-dt<1:
            cmdString = input()
            dt = t
        if len(cmdString)>0:
            print(now.strftime('%Y-%m-%d %H:%M:%S '))
            print(cmdString)
            cmdStringByte = cmdString.encode('utf-8')
            arduino.write(cmdStringByte)
            f.write(now.strftime('%Y-%m-%d %H:%M:%S '))
            f.write(cmdString)


    #RX
        dataString = arduino.readline()
        dataString_decoded = dataString.decode()[:len(dataString)-1]
        print(now.strftime('%Y-%m-%d %H:%M:%S '))
        print(dataString_decoded)
        f.write(now.strftime('%Y-%m-%d %H:%M:%S '))
        f.write(dataString_decoded)
        f.close()
            

