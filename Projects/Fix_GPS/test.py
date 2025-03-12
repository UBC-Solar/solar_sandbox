import serial

# Open the serial port
ser = serial.Serial('/dev/ttyACM0', 230400)


while True:
    print(ser.readline().decode('utf-8'))

