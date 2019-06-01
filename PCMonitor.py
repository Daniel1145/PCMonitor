import atexit
import signal
import serial
import psutil
import time


def handle_exit():
    ser.close()


atexit.register(handle_exit)
signal.signal(signal.SIGTERM, handle_exit)
signal.signal(signal.SIGINT, handle_exit)

ser = serial.Serial('COM5', 9600)

while(True):
    time.sleep(2)
    cpuUsage = psutil.cpu_percent(None, False)
    print(ser.write(bytes('*' + str(cpuUsage) + ',', 'utf-8')))

    ramUsage = psutil.virtual_memory()
    ramUsed = ramUsage.used / (2**30)
    ramUsed = round(ramUsed, 1)
    ramTotal = ramUsage.total / (2**30)
    ramTotal = round(ramTotal, 1)
    ser.write(bytes(str(ramUsed) + ',', 'utf-8'))
    ser.write(bytes(str(ramTotal) + '^', 'utf-8'))
    print(str(cpuUsage) + "%")
    print(str(ramUsed) + "GB")



