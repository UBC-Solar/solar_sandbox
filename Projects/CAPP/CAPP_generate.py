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
    def __init__(self, name, datatype, unit=None):
        self.name = name
        self.datatype = datatype
        self.unit = unit

# Supported datatypes and their size in bytes
supported_datatypes = {"uint8_t": 1, 
                       "int8_t": 1, 
                       "uint16_t": 2,
                       "int16_t": 2, 
                       "uint32_t": 4, 
                       "int32_t": 4, 
                       "uint64_t": 8, 
                       "int64_t": 8, 
                       "float": 4, 
                       "double": 8
                    }




# Specify the path to your repository (or use '.' for the current directory)
repo_path = '.'
project_path = './Projects/CAPP/'

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

def write_common_header(file):
    header = ""

    header += "// ***WARNING***\n"
    header += "// THIS FILE IS AUTO-GENERATED\n"
    header += "// ANY MANUAL CHANGES HERE WILL BE OVERWRITTEN\n\n"

    file.write(header)

def write_CAPP_c_header(file, CAN_messages):
    header = ""
    header += "#include <stdint.h>\n"
    header += "#include \"CAPP.h\"\n\n"

    header += "// Local variables\n"

    for CAN_message in CAN_messages:
        header += f"{CAN_message.name}_t {CAN_message.name} = {{0}};\n"
    
    header += "\n"

    file.write(header)

def write_CAPP_extract_functions(file):
    extract_functions = ""

    for datatype in supported_datatypes:
        extract_functions += f"""{datatype} extract_{datatype}(uint8_t* data, uint8_t start_byte)
{{
   union {{
      uint8_t bytes[{supported_datatypes[datatype]}];
      {datatype} value;
   }} converter;

   for (int i = 0; i < {supported_datatypes[datatype]}; i++) {{
      converter.bytes[i] = data[start_byte + i];
   }}

   return converter.value;
}}

"""

    file.write(extract_functions)


def write_CAN_message_struct(file, CAN_message):
    struct = ""
    struct += f"#define {CAN_message.name.upper()}_CAN_ID (0x{CAN_message.id:X})\n"
    struct += "typedef struct {\n"

    for signal in CAN_message.signals:
        struct += f"   {signal.datatype} {signal.name};\n"

    struct += f"}} CAPP_{CAN_message.name}_t;\n\n"

    file.write(struct)


def write_CAN_Message_parser(file, CAN_message):
    parser = ""
    parser += f"void CAPP_Parse_{CAN_message.name}(uint8_t* data)\n"
    parser += "{\n"
    start_byte = 0
    for signal in CAN_message.signals:
        parser += f"   {CAN_message.name}.signal = extract_{signal.datatype}(data, {start_byte});\n"
        start_byte += supported_datatypes[signal.datatype]
    parser += "}\n\n"

    file.write(parser)

def write_CAN_parser(file, CAN_messages):
    parser = ""
    parser += "void CAPP_Rx_CAN_Message(uint8_t CAN_ID, uint8_t* data)\n"
    parser += "{\n"
    parser += "   switch(CAN_ID)\n"
    parser += "   {\n"
    for CAN_message in CAN_messages:
        parser += f"      case({CAN_message.name.upper()}_CAN_ID):\n"
        parser += f"         CAPP_Parse_{CAN_message.name}(data);\n"
        parser += f"         break;\n"
    parser += "   }\n"
    parser += "}\n"
    
    file.write(parser)

# Generate struct typedefs
with open(project_path + "CAPP.c", "w") as CAPP_c:
    write_common_header(CAPP_c)
    write_CAPP_c_header(CAPP_c, CAN_messages)
    write_CAPP_extract_functions(CAPP_c)

    for CAN_message in CAN_messages:
        write_CAN_Message_parser(CAPP_c, CAN_message)

    write_CAN_parser(CAPP_c, CAN_messages)



with open(project_path + "CAPP.h", "w") as CAPP_h:
    write_common_header(CAPP_h)

    for CAN_message in CAN_messages:
        write_CAN_message_struct(CAPP_h, CAN_message)
