import matplotlib.pyplot
import dill
import matplotlib
import numpy as np
from data_tools.query import DBClient
from data_tools.collections import TimeSeries
import can
import time
import struct


def float_to_int16_bytes_array(flt: float) -> list[int]:
    # 1) Convert float → signed 16‑bit int (clamped to [-32768, 32767])
    i16 = max(min(int(flt),  32767), -32768)

    # 2) Pack into 2 bytes (big‑endian: high byte first)
    packed = struct.pack('>h', i16)
    high_byte, low_byte = packed[0], packed[1]

    # 3) Build a 7‑element list: first two from our int16, rest zeros
    arr = [
        low_byte,     # byte 0
        high_byte,    # byte 1
        0x00,         # byte 2 (padding/example)
        0x00,         # byte 3
        0x00,         # byte 4
        0x00,         # byte 5
        0x00,         # byte 6
    ]

    return arr

def float_to_uint16_bytes_array(flt: float) -> list[str]:
    # 1) Convert float → unsigned 16‑bit int, clamped to [0, 65535]
    u16 = int(flt)
    if u16 < 0:
        u16 = 0
    elif u16 > 0xFFFF:
        u16 = 0xFFFF

    # 2) Pack into 2 bytes (big‑endian: high byte first)
    packed = struct.pack('>H', u16)
    high_byte, low_byte = packed[0], packed[1]

    # 3) Build a 6‑element list: first two from our uint16, rest zeros
    arr = [
        low_byte,    # byte 0
        high_byte,     # byte 1
        0x00,         # byte 2 (padding/example)
        0x00,         # byte 3
        0x00,         # byte 4
        0x00,         # byte 5
    ]

    return arr

f = open("hppc_data.csv", "w")
# bus = can.interface.Bus(channel='can0', bustype='socketcan')

PACK_CURRENT_ID     = 0x450
PACK_VOLTAGE_ID     = 0x623


with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
    pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
    current_data = dill.load(pack_curr_f)
    voltage_data = dill.load(pack_voltage_f)

    # for i in range(10):
    for i in range(len(current_data)):
        # Convert current float to int16_t
        current_as_int = float_to_int16_bytes_array(current_data[i] * 65.535)
        voltage_as_int = float_to_uint16_bytes_array(voltage_data[i] * 468)

        # print(current_data[i], current_as_int, voltage_data[i], voltage_as_int)

        # Send pack voltage
        voltage_message = can.Message(arbitration_id=PACK_VOLTAGE_ID, data=voltage_as_int, is_extended_id=False)

        # Send pack current
        current_message = can.Message(arbitration_id=PACK_CURRENT_ID, data=current_as_int, is_extended_id=False)

    

f.close()
