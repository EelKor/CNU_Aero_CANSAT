f=open("cansatData.txt",'a')
f.close

import datetime, serial, sys, time, msvcrt

arduino = serial.Serial(port = "COM8", baudrate = 9600)

while True:
    if arduino.readable():
        now = datetime.datetime.now()
        f=open("cansatData.txt",'a', encoding = "UTF8")

        #RX
        dataString = arduino.readline()
        dataString_decoded = dataString.decode()[:len(dataString)-1]
        print(now.strftime('%Y-%m-%d %H:%M:%S '))
        print(dataString_decoded)
        f.write(now.strftime('%Y-%m-%d %H:%M:%S '))
        f.write(dataString_decoded)
        f.close()

    def readInput(caption, default, timeout = 1):
        if caption != '':
            print(caption)
        start_time = time.time()
        input = ''
        while True:
            if msvcrt.kbhit():
                byte_arr = msvcrt.getche()
                if ord(byte_arr) == 13:
                    break
                elif ord(byte_arr) >= 32:
                    input += "".join(map(chr,byte_arr))
            if len(input) ==0 or (time.time()-start_time) > timeout:
                break
        if len(input)>0:
            return input
        else:
            return default

    cmdString = readInput('','', 1)
    arduino.write(cmdString.encode())
    print(now.strftime('%Y-%m-%d %H:%M:%S '))
    print(cmdString)
    f.write(now.strftime('%Y-%m-%d %H:%M:%S '))
    f.write(cmdString)
    f.close()
