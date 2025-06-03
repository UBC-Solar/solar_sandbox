import serial
import struct

ser = serial.Serial('/dev/ttyUSB0', baudrate=2000000)

while True:
    x = ser.read(8)
    y = struct.unpack('<H', x[:2])[0]
    print(ser.read(8).hex(), y)
