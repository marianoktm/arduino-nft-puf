import serial
import os.path
import time

device_file = '/dev/ttyACM0'

while False == os.path.isfile(device_file):
    time.sleep(0.5)
    continue

# open serial port
ser = serial.Serial(device_file, 115200)

# check which port was really used
# print(ser.name)

# write a string
ser.write(b'p')



# read output
outpuf = ser.readline()
out_str = str(outpuf)[2:-3]
print(out_str)
print('\n\n')

# close port
ser.close()