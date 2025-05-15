import numpy as np
from data_tools.collections import TimeSeries
import dill
import numpy as np
import matplotlib

# Assumes soc.csv has one float per line, no header
pack_voltage_f = open("/home/aarjav/Documents/UBC/solar_sandbox/Projects/SoC_Algorithm/voltage_data_comp.dill", "rb")
voltage_data = dill.load(pack_voltage_f)
meta = voltage_data._meta
meta["start"] = voltage_data._start
meta["stop"] = voltage_data._stop
meta["units"] = ""
meta["granularity"] = 0.1
meta["length"] = len(voltage_data)
meta["length"] = len(voltage_data)
meta['measurement'] = "BMS"
meta['field'] = "SoC"

meta_v = voltage_data._meta
meta_v["start"] = voltage_data._start
meta_v["stop"] = voltage_data._stop
meta_v["units"] = ""
meta_v["granularity"] = 0.1
meta_v["length"] = len(voltage_data)
meta_v["length"] = len(voltage_data)
meta_v['measurement'] = "BMS"
meta_v['field'] = "Voltage"


soc_array = TimeSeries(np.loadtxt('/home/aarjav/Documents/UBC/physics/examples/kalman_filter_examples/soc.csv', dtype=float), meta=meta)
voltage_data, soc_array = TimeSeries.align(voltage_data, soc_array)

soc_array.plot(show=False)
matplotlib.pyplot.savefig("image.png")

# voltage_array = TimeSeries(np.loadtxt('/home/aarjav/Documents/UBC/physics/examples/kalman_filter_examples/last_voltage.csv', dtype=float), meta=meta_v)
# voltage_data, voltage_array = TimeSeries.align(voltage_data, voltage_array)

# voltage_array.plot(show=False)
# matplotlib.pyplot.savefig("image_voltage.png")


print(len(soc_array))
