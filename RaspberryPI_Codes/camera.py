from time import sleep
import picamera

camera = picamera.PiCamera()

camera.resolution = (640,480)
camera.start_recording(output = 'test.h264')
camera.wait_recording(1200)
camera.stop_recording()

