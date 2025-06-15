import serial
import time

start_time = time.time()

csv = open("data.csv", "w")

def read():
    s = serial.Serial('COM9', 115200)
        
    # Read from COM9 in a while loop
    while True:
        data = s.read(1)
        
        value = int.from_bytes(data, 'little')

        adc1 = (value >> 48) & 0xFFFF
        adc2 = (value >> 32) & 0xFFFF
        
        adc1 = adc1 * 3.3 / 4095
        adc2 = adc2 * 3.3 / 4095
        
        timer_in_s_since_start = time.time() - start_time
        
        print(f"ADC1: {adc1}, ADC2: {adc2}, Timer: {timer_in_s_since_start}")
        
        csv.write(f"{adc1},{adc2}\n")
        
        # Sometimes this function gives garbage, just reset when it happens
        if adc1 > 5000 or adc2 > 5000: return
        
while True:
    read()
    print("Restarted")