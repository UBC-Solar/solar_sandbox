import serial
import time

start_time = time.time()

csv = open("data.csv", "w")

def read():
    s = serial.Serial('COM9', 115200)
    seconds = 0
    iters = 0
        
    # Read from COM9 in a while loop
    while True:
        data = s.read(8)
        
        value = int.from_bytes(data, 'little')

        adc1 = (value >> 48) & 0xFFFF
        adc2 = (value >> 32) & 0xFFFF
        stm_timer = value & 0xFFFFFFFF # Note the STM32 timer is unreliable for some reason
        
        adc1 = adc1 * 3.3 / 4095
        adc2 = adc2 * 3.3 / 4095
        
        stm_timer_s = stm_timer / 100_000_000 # convert us to s
        timer_in_s_since_start = time.time() - start_time
        
        if (time.time() - start_time - seconds > 1):
            seconds += 1
            print(f"Iterations per second: {iters}")
            print(f"ADC1: {adc1}, ADC2: {adc2}, Timer: {stm_timer_s}, {timer_in_s_since_start}")
            iters = 0
        iters += 1
        
        csv.write(f"{adc1},{adc2},{timer_in_s_since_start}\n")
        
        # Sometimes this function gives garbage, just reset when it happens
        if adc1 > 5000 or adc2 > 5000: return
        
while True:
    read()
    print("Restarted")