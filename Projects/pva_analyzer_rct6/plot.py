import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('/home/aarjav/Documents/UBC/solar_sandbox/data.csv')

# Plot Voltage (x) vs Voltage (y)
plt.figure(figsize=(8, 6))
plt.plot(df['Time'], df['Voltage'], marker='o', linestyle='-')
plt.xlabel('Time (V)')
plt.ylabel('Voltage (A)')
plt.title('Voltage vs Time')
plt.grid(True)
plt.tight_layout()
plt.savefig('plot.png')
# plt.show()
