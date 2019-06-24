import atexit
import signal
import serial
import psutil
import time
import clr
import sys
import os

hwtypes = ['Mainboard','SuperIO','CPU','RAM','GpuNvidia','GpuAti','TBalancer','Heatmaster','HDD']
sensortypes = ['Voltage','Clock','Temperature','Load','Fan','Flow','Control','Level','Factor','Power','Data','SmallData']

def initialize_openhardwaremonitor():
    file = os.getcwd() + '\\dll\\OpenHardwareMonitorLib.dll'
    clr.AddReference(file)

    from OpenHardwareMonitor import Hardware

    handle = Hardware.Computer()
    handle.MainboardEnabled = True
    handle.CPUEnabled = True
    handle.RAMEnabled = True
    handle.GPUEnabled = True
    handle.HDDEnabled = True
    handle.Open()
    return handle

def write_gpu(handle):
    for i in handle.Hardware:
        i.Update()
        for sensor in i.Sensors:
            if hwtypes[sensor.Hardware.HardwareType] == 'GpuAti' and sensor.SensorType == sensortypes.index('Load'):
                ser.write(bytes(str(sensor.Value) + '^', 'utf-8'))
            elif hwtypes[sensor.Hardware.HardwareType] == 'GpuAti' and sensor.SensorType == sensortypes.index('Temperature'):
                ser.write(bytes(str(sensor.Value) + ',', 'utf-8'))

def handle_exit():
    ser.close()

if __name__ == "__main__":
    atexit.register(handle_exit)
    signal.signal(signal.SIGTERM, handle_exit)
    signal.signal(signal.SIGINT, handle_exit)

    ser = serial.Serial('COM5', 9600)

    HardwareHandle = initialize_openhardwaremonitor()

    while(True):
        time.sleep(1)
        cpuUsage = psutil.cpu_percent(None, False)
        ser.write(bytes('*' + str(cpuUsage) + ',', 'utf-8'))

        ramUsage = psutil.virtual_memory()
        ramUsed = ramUsage.used / (2**30)
        ramUsed = round(ramUsed, 1)
        ramTotal = ramUsage.total / (2**30)
        ramTotal = round(ramTotal, 1)
        ser.write(bytes(str(ramUsed) + ',', 'utf-8'))
        ser.write(bytes(str(ramTotal) + ',', 'utf-8'))
        write_gpu(HardwareHandle)
