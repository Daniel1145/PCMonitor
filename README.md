# PCMonitor
A simple Arduino program that uses Python to read PC information (such as CPU, RAM, and GPU usage and temperatures) and write it to 
the Arduino UNO, which displays it on an LCD screen.

## Getting Started

### Components Used

- 1x Arduino UNO
- 1x Kuman 320x480 LCD Display

### Installing

#### Required Libraries
- Adafruit_GFX
- Display Library (For your display)
- Pyserial
- Pythonnet
- psutil
- OpenHardwareMonitor (From https://openhardwaremonitor.org/downloads/)

First, make sure to install the Adafruit_GFX library on Arduino if you haven't already. Also, make sure to install the specific library for
your display (You may need to change the code to get it to work with a different display).

Next, in the Python script make sure that the correct serial port is opened on line 16. This can be found in Arduino under Tools->Port. Also, make sure to create a folder name "dll" in the same directory as your python script, and put the OpenHardwareMonitorLib.dll file into the folder.

Finally, with the LCD connected to the Arduino UNO, upload the Arduino code to the Arduino UNO, and run the Python script on your PC. This
should get the Arduino UNO to display your PC information on the LCD!

## TODO

- Add CPU Temperature monitoring
- Add graphical views of information
