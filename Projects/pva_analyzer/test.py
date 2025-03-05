import serial
import time

MAX = 50
LEN = 8196
DELIM = b'\xff\xff\x0a\xff' 

ser = serial.Serial('/dev/ttyACM0', 2000000)
mega_list = []
new_val = 0

val = ser.read(MAX * LEN)

x = val.split(DELIM)

truth = []

for min in x:
    # print(len(min))print(val.hex(), file=file)

    if (len(min) == (LEN - 4)):
        truth.append(min)

file = open('data.txt', 'w')

data_all = []
for thing in truth:
    data_all = []
    for i in range(0, len(thing), 32):
        dat = int.from_bytes(thing[i:i+2], byteorder='little')
        data_all.append(dat)
        print(dat, file=file)
    print(max(data_all))
    # uint16_data = [int.from_bytes(thing[i:i+2], byteorder='big') for i in range(0, len(thing), 2)]
    # print(len(uint16_data), max(uint16_data))
    # for data in uint16_data:

# while True:
    # val = ser.read(2)
    # if ((val.hex() == "ffff") or (val.hex() == "ff0a")):
    #     new_val = ser.read(8196)
    #     mega_list.append(new_val)
    #     if (len(mega_list) == MAX):
    #         break
        # print(new_val.hex())

    # print(len(val))
    # print(len(mega_list))
    # val = int.from_bytes(ser.readline()[:-1], byteorder="big")
    # print(val)
    # val = ser.read()
    # print(ser.read(4096 * 2 + 4).hex())
    # print(ser.readline().decode('utf-8'))



# # Parse the mega list
# parsed_data = []
# for item in mega_list:
    # Remove the last 4 bytes (garbage)
    # valid_data = item[:-4]
    # # Convert the valid data to uint16s
    # uint16_data = [int.from_bytes(valid_data[i:i+2], byteorder='big') for i in range(0, len(valid_data), 2)]
    # parsed_data.append(uint16_data)

# # Now parsed_data contains the parsed uint16 values
# print(parsed_data)
# print(len(parsed_data[0]))
