import matplotlib.pyplot
import dill
import matplotlib
import numpy as np
from data_tools.query import DBClient
from data_tools.collections import TimeSeries
from datetime import datetime
import re

### read /home/aarjav/output.txt


file = open("/home/aarjav/output.txt")

def parse_timestamp(line):
    # Extract the float value between the parentheses
    return float(line.split(')')[0].strip(' ('))

def parse_soc(line):
    # Extract the float value between the parentheses
    return line.split(' ')

def compute_differences(file_path):
    output = open("soc2.csv", "w")
    count = 0
    with open(file_path, 'r') as f:
        for line in f:
            count += 1
            print(count)
            soc = int(parse_soc(line)[-1][:-1], 16)
            print(soc / 100.0, file=output)
            print(soc / 100.0, file=output)
            

# def compute_differences(file_path):
#     with open(file_path, 'r') as f:
#         previous_time = None  
#         for line in f:
#             timestamp = parse_timestamp(line)
#             if previous_time is not None:
#                 delta = timestamp - previous_time
#                 if not (0.2 < delta < 0.22):
#                     print(f"Δt = {delta:.6f} s")
#             previous_time = timestamp

# Usage
compute_differences("/home/aarjav/output.txt")

