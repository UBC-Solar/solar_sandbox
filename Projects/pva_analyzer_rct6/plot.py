import pandas as pd
import matplotlib.pyplot as plt
import mplcursors

# Read the CSV file
df = pd.read_csv('data.csv')

# Plot Voltage (x) vs Power (y)
plt.figure(figsize=(8, 6))
line = plt.plot(df['Voltage'], df['Power'], marker='o', linestyle='None')
cursor = mplcursors.cursor(line, hover=True)

@cursor.connect("add")
def _(sel):
    x,y=sel.target
    sel.annotation.set_text(
        f"{y:.2f}"
    )
    bbox = sel.annotation.get_bbox_patch()
    bbox.set_facecolor("white")
    bbox.set_edgecolor("black")
    bbox.set_alpha(0.8)
    bbox.set_boxstyle("round,pad=0.3")
plt.xlabel('Voltage (V)')
plt.ylabel('Power (W)')
plt.title('Power vs Voltage')
plt.grid(True)
plt.tight_layout()
#plt.savefig('plotIV.png')
plt.show()

# Plot Voltage (x) vs Current (y)
plt.figure(figsize=(8, 6))
line = plt.plot(df['Current'], df['Voltage'], marker='o', linestyle='None')
cursor = mplcursors.cursor(line, hover=True)

@cursor.connect("add")
def _(sel):
    x,y=sel.target
    sel.annotation.set_text(
        f"{y:.2f}"
    )
    bbox = sel.annotation.get_bbox_patch()
    bbox.set_facecolor("white")
    bbox.set_edgecolor("black")
    bbox.set_alpha(0.8)
    bbox.set_boxstyle("round,pad=0.3")
plt.xlabel('Voltage (V)')
plt.ylabel('Current (A)')
plt.title('Current vs Voltage')
plt.grid(True)
plt.tight_layout()
#plt.savefig('plotIV.png')
plt.show()

# Plot Time (x) vs Voltage (y)
plt.figure(figsize=(8, 6))
line = plt.plot(df['Time'], df['Voltage'], marker='o', linestyle='None')
cursor = mplcursors.cursor(line, hover=True)

@cursor.connect("add")
def _(sel):
    x,y=sel.target
    sel.annotation.set_text(
        f"{y:.2f}"
    )
    bbox = sel.annotation.get_bbox_patch()
    bbox.set_facecolor("white")
    bbox.set_edgecolor("black")
    bbox.set_alpha(0.8)
    bbox.set_boxstyle("round,pad=0.3")
plt.xlabel('Time (Samples)')
plt.ylabel('Voltage (V)')
plt.title('Voltage vs Time')
plt.grid(True)
plt.tight_layout()
#plt.savefig('plotTV.png')
plt.show()

# Plot Time (x) vs Current (y)
plt.figure(figsize=(8, 6))
line = plt.plot(df['Time'], df['Current'], marker='o', linestyle='None')
cursor = mplcursors.cursor(line, hover=True)

@cursor.connect("add")
def _(sel):
    x,y=sel.target
    sel.annotation.set_text(
        f"{y:.2f}"
    )
    bbox = sel.annotation.get_bbox_patch()
    bbox.set_facecolor("white")
    bbox.set_edgecolor("black")
    bbox.set_alpha(0.8)
    bbox.set_boxstyle("round,pad=0.3")
plt.xlabel('Time (Samples)')
plt.ylabel('Current (A)')
plt.title('Current vs Time')
plt.grid(True)
plt.tight_layout()
#plt.savefig('plotIT.png')
plt.show()