import serial
import time
from dbAccess import DBAccess

# 시리얼 통신 UART2
loraTX = serial.Serial('/dev/ttyAMA3' , 9600)

# Lora 모듈 파라미터 설정
AT_ADDRESS = "AT+ADDRESS=76" + "\r\n"
AT_NETWORKID = "AT+NETWORKID=2" + "\r\n"
AT_PARAMETER = "AT+PARAMETER=12,7,1,4" + "\r\n"

loraTX.write(AT_ADDRESS.encode())
time.sleep(0.2)
loraTX.write(AT_NETWORKID.encode())
time.sleep(0.1)
loraTX.write(AT_PARAMETER.encode())
time.sleep(0.1)

for i in range(3):
	if loraTX.readable():
		AT_result = loraTX.readline().decode()[:-2]
		print(AT_result)
		time.sleep(0.1)

while True:
	db = DBAccess()
	TXData = db.readone()[0]
	SOS = db.readSOS()
	Notice = db.readNotice()
	db.close()
	com =  str(TXData['pressure']) + "," + str(TXData['alt']) + "," + str(TXData['altGPS']) \
			+ ","  + str(TXData['speed']) + "," + \
			 str(TXData['roll']) +"," + str(TXData['pitch']) + "," \
		    + str(TXData['lat']) + "," + str(TXData['lng']) + "," + str(len(SOS)) + "," + str(len(Notice))	
	msg = "AT+SEND=77," + str(len(com)) + "," + com + "\r\n"
	print(msg)
	loraTX.write(msg.encode())
	
	if loraTX.readable():
		rx = loraTX.readline()
		print(rx.decode()[:-2].split(","))
