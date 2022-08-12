import serial
import time

# 시리얼 통신 UART2
UART3 = serial.Serial('/dev/ttyAMA1' , 115200)


lastUpdateT = time.time()

while True:

	if(UART3.readable()):
		print(UART3.readline())
