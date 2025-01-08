import yaml
from git import Repo

class CAN_Message:
    def __init__(self, id, name, sender, receivers, signals):
        self.id = id
        self.name = name
        self.sender = sender
        self.receivers = receivers
        self.signals = signals


class CAN_Signal:
    def __init__(self, name, datatype, unit):
        self.name = name
        self.datatype = datatype
        self.unit = unit

# Specify the path to your repository (or use '.' for the current directory)
repo_path = '.'

# Open the repository
repo = Repo(repo_path)

# Get the root path of the Git repository. This ensure that this file can be called from anywhere.
root_path = repo.git.rev_parse("--show-toplevel")
print(f"Root path of the Git repository: {root_path}")

with open(f"{root_path}/Projects/CAPP/CAPP.yaml", "r") as yaml_file:
    yaml_data = yaml.safe_load(yaml_file)


# Iterate through the YAML data to create objects
CAN_messages = []

# Assuming 'data' contains the loaded YAML data
for CAN_message in yaml_data["CAN_Messages"]:
    signals = [
        CAN_Signal(
            name=signal["name"],
            datatype=signal["datatype"],
            unit=signal["unit"]
        )
        for signal in CAN_message["signals"]
    ]
    
    CAN_message = CAN_Message(
        name      = CAN_message["name"],
        id        = CAN_message["id"],
        sender    = CAN_message["sender"],
        receivers = CAN_message["receivers"],
        signals   = signals
    )
    
    CAN_messages.append(CAN_message)


    # bit 0: R/W
    # bits 1-3: register address
    # bit 4-10: data
    # bit 11-15: crc
