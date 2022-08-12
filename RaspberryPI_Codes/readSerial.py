import serial
import time
from dbAccess import DBAccess

# 시리얼 통신 UART2
UART3 = serial.Serial('/dev/ttyAMA1' , 9600)

lastUpdateT = time.time()

while True:
	if(UART3.readable()):
		try:
			db = DBAccess()
			raw = UART3.readline().decode()[:-2]
			data = raw.split(",")
			intData = list(map(int,data[0:7]))
			floatData = list(map(float, data[7:14]))
			strData = list(map(str, data[14:]))
			print(intData + floatData + strData)
			db.upload(intData + floatData + strData)
			db.close()
		except:
			if(UART3.readable()):
				print(UART3.readline())

