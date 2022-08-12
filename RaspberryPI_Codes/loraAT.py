import serial
import time
from dbAccess import DBAccess

# 시리얼 통신 UART2
UART3 = serial.Serial('/dev/ttyAMA2' , 9600)


while True:
	com = input("com:" )
	com = com + "\r\n"
	UART3.write(com.encode())
	if UART3.readable():
		response = UART3.readline().decode().split(",")
		print(response)

	
