import serial
import time
from dbAccess import DBAccess
from threading import Thread

# 시리얼 통신 UART2
#UART3 = serial.Serial('/dev/ttyAMA3' , 9600) # ADDRESS = 76
UART2 = serial.Serial('/dev/ttyAMA2' , 9600) # ADDRESS = 75

db = DBAccess()

lastUpdateT = time.time()

while True:

#if UART3.readable():
#		response = UART3.readline().decode().split(",")
#		print("ADD76:", response)

	if UART2.readable():
		response = UART2.readline().decode().split(",")
		print("ADD75:", response)
	
