import matplotlib.pyplot
import dill
import matplotlib
import numpy as np
from data_tools.query import DBClient
from data_tools.collections import TimeSeries

f = open("hppc_data.csv", "w")

with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
    pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
    current_data = dill.load(pack_curr_f)
    voltage_data = dill.load(pack_voltage_f)

    # for i in range(len(current_data)):
    for i in range(len(current_data)):
        print(current_data[i], voltage_data[i], file=f)

f.close()

# f = open("hppc_data.csv", "w")

# with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
#     pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
#     current_data = dill.load(pack_curr_f)
#     voltage_data = dill.load(pack_voltage_f)

#     # for i in range(len(current_data)):
#     for i in range(len(current_data)):
#         print(current_data[i], voltage_data[i], file=f)

# f.close()


from data_tools.collections import TimeSeries

client = DBClient()

# ISO 8601-compliant times corresponding to pre-competition testing
start = "2024-07-16T14:00:00Z"
stop = "2024-07-17T01:00:00Z"

# We can, in one line, make a query to InfluxDB and parse
# the data into a powerful format: the `TimeSeries` class.
voltage_data: TimeSeries = client.query_time_series(
    start=start,
    stop=stop,
    field="TotalPackVoltage",
    units="V"
)
current_data: TimeSeries = client.query_time_series(
    start=start,
    stop=stop,
    field="PackCurrent",
    units="C"
)

# Plot the data
voltage_data.plot()