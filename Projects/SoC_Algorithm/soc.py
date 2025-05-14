import matplotlib.pyplot
import dill
import matplotlib
import numpy as np
from data_tools.query import DBClient
from data_tools.collections import TimeSeries
from datetime import datetime

### HPPC

# f = open("hppc_data.csv", "w")

# with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
#     pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
#     current_data = dill.load(pack_curr_f)
#     voltage_data = dill.load(pack_voltage_f)

#     # for i in range(len(current_data)):
#     for i in range(len(current_data)):
#         print(current_data[i], voltage_data[i], file=f)

# f.close()

# f = open("hppc_data.csv", "w")

# with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
#     pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
#     current_data = dill.load(pack_curr_f)
#     voltage_data = dill.load(pack_voltage_f)

#     # for i in range(len(current_data)):
#     for i in range(len(current_data)):
#         print(current_data[i], voltage_data[i], file=f)

# f.close()


### FSGP

f = open("fsgp_data.csv", "w")

with open("/home/aarjav/Documents/UBC/solar_sandbox/Projects/SoC_Algorithm/current_data_comp.dill", "rb") as pack_curr_f:
    pack_voltage_f = open("/home/aarjav/Documents/UBC/solar_sandbox/Projects/SoC_Algorithm/voltage_data_comp.dill", "rb")
    current_data = dill.load(pack_curr_f)
    voltage_data = dill.load(pack_voltage_f)

    # for i in range(len(current_data)):
    print(len(current_data))
    print(len(voltage_data))
    # for i in range(len(current_data)):
    #     print(current_data[i], voltage_data[i], file=f)

# f.close()


# from data_tools.collections import TimeSeries

# client = DBClient()

# # ISO 8601-compliant times corresponding to pre-competition testing
# start = "2024-07-16T14:00:00Z"
# stop = "2024-07-17T01:00:00Z"

# start_dt = datetime.fromisoformat(start)
# stop_dt  = datetime.fromisoformat(stop)

# # We can, in one line, make a query to InfluxDB and parse
# # the data into a powerful format: the `TimeSeries` class.
# voltage_data: TimeSeries = client.query_time_series(
#     start=start_dt,
#     stop=stop_dt,
#     field="TotalPackVoltage",
#     units="V"
# )
# current_data: TimeSeries = client.query_time_series(
#     start=start_dt,
#     stop=stop_dt,
#     field="PackCurrent",
#     units="C"
# )

# current_data, voltage_data = TimeSeries.align(current_data, voltage_data)

# current_dump_f = open("current_data_comp.dill", "wb")
# voltage_dump_f = open("voltage_data_comp.dill", "wb")

# # Align them

# dill.dump(current_data, current_dump_f)
# dill.dump(voltage_data, voltage_dump_f)

# current_dump_f.close()
# voltage_dump_f.close()

# Plot the data
# voltage_data.plot(show=False)
# matplotlib.pyplot.savefig("image2.png")
# current_data.plot(show=False)