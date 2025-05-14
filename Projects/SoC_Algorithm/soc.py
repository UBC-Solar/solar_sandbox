import matplotlib.pyplot
import dill
import matplotlib

f = open("hppc_data.csv", "w")

with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
    pack_voltage_f = open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_voltage_aligned.dill", "rb")
    current_data = dill.load(pack_curr_f)
    voltage_data = dill.load(pack_voltage_f)

    # for i in range(len(current_data)):
    for i in range(10):
        print(current_data[i], voltage_data[i], file=f)

f.close()

    # voltage_data.plot(show=False)
    # matplotlib.pyplot.savefig("image.png")

# with open("/home/aarjav/Documents/UBC/sunlink/data_analysis/parameter_extraction/pack_current_aligned.dill", "rb") as pack_curr_f:
    # data = dill.load(pack_curr_f)
    # data.plot(show=False)
    # matplotlib.pyplot.savefig("image.png")

    # print(len(data))


