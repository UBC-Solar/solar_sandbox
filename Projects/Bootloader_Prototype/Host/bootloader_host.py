import serial 
import time

file_path = "bin files/user_application.bin"
baud_rate = 115200
serial_port = "/dev/ttyACM0"

STATUS_GOOD = bytes.fromhex('01')
COMMAND_JUMP = bytes.fromhex('01')
COMMAND_FLASH = bytes.fromhex('02')

COMMAND_FINISH_FLASH = bytes.fromhex("0400")
COMMAND_FLASH_CHUNK = bytes.fromhex('05')

ACK_SIZE = 1


def main():
    with serial.Serial(serial_port, baud_rate) as ser:
        print("="*60)
        print(" " * 18 + " Bootloader Host App ")
        print("="*60)
        print(" Input 0 to send default status message")
        print(" Input 1 to Flash a new program")
        print("="*60)

        val = int(input("Input response: "))
        if val != 1:
            ser.write(COMMAND_JUMP)
        else:
            ser.write(COMMAND_FLASH)
            write_startup_ack = ser.read(ACK_SIZE)
            print(write_startup_ack)

            if write_startup_ack == STATUS_GOOD:
                print("\nmade to file write")
                transmit_binary(file_path, 128, ser)

class TransmissionError(Exception):
    """Custom exception for handling transmission errors."""
    pass

def transmit_binary(file_path, chunk_size, ser):
    try:
        with open(file_path, "rb") as file:
            chunk = file.read(chunk_size)
            while chunk:

                # Send the chunk to the serial device
                time.sleep(0.01)

                chunk_send = COMMAND_FLASH_CHUNK + len(chunk).to_bytes(1, byteorder='big') + chunk
                
                print(len(chunk_send))
                print('\n')
                print(chunk_send)

                ser.write(chunk_send) 

                # Read the acknowledgment status
                ack_status = ser.read(ACK_SIZE)
                print("\n")
                print(ack_status)
                
                if ack_status == STATUS_GOOD:
                    # Read the next chunk if the transmission was successful
                    chunk = file.read(chunk_size)
                else:
                    # Raise an exception if the acknowledgment status is not good
                    raise TransmissionError("Failed to transmit chunk: Acknowledgment not received.")
        ser.write(COMMAND_FINISH_FLASH)
        print("File sent successfully.")
    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except TransmissionError as e:
        print(f"Transmission error: {e}")
    except Exception as e:
        print(f"An error occurred: {e}")

# Ensure the main function runs when the script is executed
if __name__ == "__main__":
    main()
    