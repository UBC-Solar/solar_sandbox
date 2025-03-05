import serial
import struct
import threading
import queue

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 2000000
READ_SIZE = 8196
ALIGN_DELIM = b'\xff\xff\x00\x0a'  # alignment marker: last 4 bytes must match
OUTPUT_FILE = 'data.txt'

def read_aligned_chunk(ser):
    """
    Reads from the serial port until a full 8196-byte chunk is obtained
    and the last 4 bytes exactly equal ALIGN_DELIM.
    If misalignment is detected, discards bytes to re-sync.
    """
    while True:
        # Use read_until with a max size so that if the delimiter is found early,
        # we can check whether it occurs at the proper position.
        # chunk = ser.readline()
        chunk = ser.read(100)
        # print(len(chunk))
        if ("ff" in chunk.hex()):
            print(chunk.hex())
        # If we didn’t get a full chunk, keep waiting.
        # if len(chunk) != READ_SIZE:
        #     continue
        # # Check that the last 4 bytes match the delimiter.
        # if chunk[-4:] != ALIGN_DELIM:
        #     # Misalignment detected.
        #     # Discard one byte (or more sophisticated resync may be needed)
        #     ser.read(1)
        #     continue
        # # We have a correctly aligned chunk.
        return chunk

def producer(q, ser):
    """
    Continuously reads aligned chunks from the serial port and puts them in a thread-safe queue.
    """
    while True:
        # print("hi")
        # print(ser.readline().hex())
        aligned_chunk = read_aligned_chunk(ser)
        # q.put(aligned_chunk)

def consumer(q):
    """
    Processes each aligned chunk: converts the first 8192 bytes (4096 uint16 values)
    to numbers and writes them to a file, one per line.
    """
    with open(OUTPUT_FILE, 'a') as f:
        while True:
            chunk = q.get()
            # Unpack first 8192 bytes as 4096 little-endian uint16 values.
            values = struct.unpack('<4096H', chunk[:8192])
            print(values)
            for val in values:
                print(val)
                f.write(f"{val}\n")
            f.flush()  # Ensure immediate write-out
            q.task_done()

def main():
    # Open the serial port (adjust timeout if necessary)
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
    q = queue.Queue(maxsize=10)  # Limit the queue size to avoid memory overrun

    # Start producer and consumer threads.
    prod_thread = threading.Thread(target=producer, args=(q, ser), daemon=True)
    cons_thread = threading.Thread(target=consumer, args=(q,), daemon=True)
    prod_thread.start()
    # cons_thread.start()

    try:
        while True:
            # Main thread can do other tasks or simply wait.
            pass
    except KeyboardInterrupt:
        print("Interrupted by user. Exiting...")

if __name__ == '__main__':
    main()
