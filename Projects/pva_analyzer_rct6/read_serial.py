import serial
import time
import sys

MAX_SAMPLES = 2600     # ← change this to however many samples you want

start_time = time.time()

csv = open("data.csv", "w")
csv.write("Current,Voltage,Power,Time\n")

def read():
    s = serial.Serial('COM10', 115200)
    sample = 0

    while True:
        # stop once you've hit your limit
        if sample >= MAX_SAMPLES:
            print(f"Reached {MAX_SAMPLES} samples, stopping read().")
            sys.exit()

        data = s.read(4)

        value = int.from_bytes(data, 'little')
        adc1 = (value >> 16) & 0xFFFF
        adc2 = (value >> 0)  & 0xFFFF

        # convert to volts
        adc1 = adc1 * 3.3 / 4095    # current
        adc2 = adc2 * 3.3 / 4095    # voltage
        voltage = (adc2 * 93.75) / 3.214
        current = (adc1 - 1.65) / 0.2
        power = current * voltage

        # print to console
        print(f"ADC1: {current}, ADC2: {voltage}, {power}, {sample}")

        # write to CSV
        csv.write(f"{current},{voltage},{power},{sample}\n")
        csv.flush()

        sample += 1

        # your existing garbage‐data guard
        if adc1 > 4 or adc2 > 4:
            print("Out‑of‑range reading, aborting early.")
            return

while True:
    read()
    print("Restarted")
