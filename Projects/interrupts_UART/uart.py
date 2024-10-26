import serial

# Configure the COM port
ser = serial.Serial(
    port='/dev/tty.usbserial-A23FNOIC',       # COM port (e.g., COM1, COM3, or /dev/ttyUSB0 for Linux)
    baudrate=115200,     # Baud rate (match with the device's settings)
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1          # Timeout for reading/writing in seconds
)

while True:
    s = ser.readline() #always use \r\n
    print(s)