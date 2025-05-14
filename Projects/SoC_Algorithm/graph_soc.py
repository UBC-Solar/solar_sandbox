import numpy as np
from data_tools.collections import TimeSeries
import dill
import numpy as np
import matplotlib

# Assumes soc.csv has one float per line, no header
pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
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

soc_array = TimeSeries(np.loadtxt('soc.csv', dtype=float), meta=meta)
voltage_data, soc_array = TimeSeries.align(voltage_data, soc_array)

soc_array.plot(show=False)
matplotlib.pyplot.savefig("image.png")


print(len(soc_array))
