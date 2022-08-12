import serial
import time
from dbAccess import DBAccess

# 시리얼 통신 UART2
loraRX  = serial.Serial('/dev/ttyAMA2' , 9600) # ADDRESS = 75

db = DBAccess()

AT_BAND = "AT+BAND=815000000" + "\r\n"
AT_PARA = "AT+PARAMETER=12,7,1,4" + "\r\n"
AT_ADDRESS = "AT+ADDRESS=75" + "\r\n"
AT_NETWORKID = "AT+NETWORKID=2" + "\r\n"

loraRX.write(AT_BAND.encode())
time.sleep(0.2)
loraRX.write(AT_PARA.encode())
time.sleep(0.2)
loraRX.write(AT_ADDRESS.encode())
time.sleep(0.2)
loraRX.write(AT_NETWORKID.encode())
time.sleep(0.2)

for i in range(4):
	if loraRX.readable():
		print(loraRX.readline().decode()[:-2])
		time.sleep(0.2)

while True:
	try:
		if loraRX.readable():
			response = loraRX.readline()[:-2].decode().split(",")
			uploadData = response[2:4]
			a = uploadData[1]
			uploadData[1] = a[:-2]

			print(uploadData,type(uploadData[0]))

			db = DBAccess()
			db.upload_notice(uploadData)
			db.close()
	except:
		print("error")
		print(uploadData)

		
	
