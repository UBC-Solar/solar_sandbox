# import serial

# ser = serial.Serial('/dev/cu.usbserial-120', 9600, timeout=1)
# while True:
#     line = ser.readline().decode('latin-1', errors='ignore').strip()
#     if line:
#         print(line)
        
import serial, time

ser = serial.Serial('/dev/cu.usbserial-120', 9600, timeout=0.1)
while True:
    b = ser.read(1)
    if b:
        print(b[0])           # prints 0,1,2,3,...
    time.sleep(0.05)
