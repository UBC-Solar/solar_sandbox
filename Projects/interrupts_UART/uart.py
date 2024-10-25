import serial
import time

# Configure the COM port (replace 'COM3' with your port name)
ser = serial.Serial(
    port='COM3',       # COM port (e.g., COM1, COM3, or /dev/ttyUSB0 for Linux)
    baudrate=115200,     # Baud rate (match with the device's settings)
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout=1          # Timeout for reading/writing in seconds
)

# Ensure the port is open
if ser.isOpen():
    print(f"{ser.port} is open")

# Write data to the serial port
try:
    ser.write(b'Hello, COM Port!\n')  # Use byte string to send data
    print("Data written to COM port")

    # Optional: Give the device time to process
    time.sleep(1)

except Exception as e:
    print(f"Error writing to COM port: {e}")

# Close the port
finally:
    ser.close()
    print("COM port closed")