import yaml
from git import Repo
import textwrap

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

# Supported datatypes and their size in bits
supported_datatypes = {"bool": 1,
                       "uint8_t": 8,
                       "int8_t": 8,
                       "uint16_t": 16,
                       "int16_t": 16,
                       "uint32_t": 32,
                       "int32_t": 32,
                       "uint64_t": 64,
                       "int64_t": 64,
                       "float": 32,
                       "double": 64
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
    header += "#include <stdbool.h>\n"
    header += "#include \"CAPP.h\"\n\n"

    header += "// Local variables\n"

    header += "\n"

    file.write(header)

def write_CAPP_parse_package_functions(file):
    code = ""

    for datatype in supported_datatypes:
        code += f"""\
        {datatype} CAPP_parse_{datatype}(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit)
        {{
           union {{
              uint8_t bytes[8];
              uint64_t value;
           }} buffer_u;

           union {{
              uint8_t bytes[1];
              {datatype} value;
           }} data;

           for (uint8_t i = 0; i < buf_len; i++) {{
              buffer_u.bytes[i] = buffer[i];
           }}

           buffer_u.value = (buffer_u.value >> start_bit) & (MASK({supported_datatypes[datatype]}));

           for (int i = 0; i < {supported_datatypes[datatype] * 8}; i++) {{
              data.bytes[i] = buffer_u.bytes[i];
           }}

           return data.value;
        }}
        \n
        void CAPP_package_{datatype}(uint8_t* buffer, uint8_t buf_len, uint8_t start_bit, {datatype} value)
        {{
           union {{
              uint8_t bytes[8];
              uint64_t value;
           }} buffer_u;

           union {{
              uint64_t buffer;
              {datatype} value;
           }} data = {{0}};
        
           for (uint8_t i = 0; i < buf_len; i++) {{
              buffer_u.bytes[i] = buffer[i];
           }}
        
           data.value = value;

           buffer_u.value = buffer_u.value | ((data.buffer << start_bit));
        
           for (int i = 0; i < 8; i++) {{
              buffer[i] = buffer_u.bytes[i];
           }}
        }}
        \n
"""

    file.write(textwrap.dedent(code))


def write_CAN_message_struct(file, CAN_message):
    struct = ""
    struct += f"#define {CAN_message.name.upper()}_CAN_ID (0x{CAN_message.id:X})\n"
    struct += "typedef struct {\n"

    for signal in CAN_message.signals:
        struct += f"   {signal.datatype} {signal.name};\n"

    struct += f"}} CAPP_{CAN_message.name}_t;\n\n"

    file.write(struct)


def write_CAN_Message_parser(file, CAN_message):
    code = ""
    code += f"void CAPP_Parse_{CAN_message.name}(uint8_t* data)\n"
    code += "{\n"
    start_bit = 0
    for signal in CAN_message.signals:
        code += f"   {CAN_message.name}.{signal} = CAPP_parse_{signal.datatype}(data, {start_bit});\n"
        start_bit += supported_datatypes[signal.datatype]
    code += "}\n\n"

    file.write(code)

# Generate struct typedefs
with open(project_path + "CAPP.c", "w") as CAPP_c:
    write_common_header(CAPP_c)
    write_CAPP_c_header(CAPP_c, CAN_messages)
    write_CAPP_parse_package_functions(CAPP_c)

    for CAN_message in CAN_messages:
        write_CAN_Message_parser(CAPP_c, CAN_message)



with open(project_path + "CAPP.h", "w") as CAPP_h:
    write_common_header(CAPP_h)

    for CAN_message in CAN_messages:
        write_CAN_message_struct(CAPP_h, CAN_message)
