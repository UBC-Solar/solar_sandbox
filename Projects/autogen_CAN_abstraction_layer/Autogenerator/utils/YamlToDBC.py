"""
Purpose: Extract DBC information from CAN Msg YAML and generate *.dbc 
    Note: We use a dictionary as the format to communicate information.
Author: Aarjav Jain
Date: 2024-08-29
"""


import yaml
import cantools
from cantools.database.can import Signal, Message
from cantools.database.conversion import LinearIntegerConversion
from ParseYaml import YAML_read

can_dict = YAML_read()

# Create a CAN database
db = cantools.db.Database()

# Add messages to the database
for msg in can_dict['CAN_Messages']:
    signals = []
    for sig in msg['signals']:
        signal = Signal(
            name=sig['name'],
            start=sig['start_bit'],
            length=sig['bit_length'],
            byte_order=sig['byte_order'],
            is_signed=sig['is_signed'],
            conversion=LinearIntegerConversion(sig['factor'], sig['offset']),
            minimum=sig['min'],
            maximum=sig['max'],
            unit=sig['unit']
        )
        signals.append(signal)
    
    message = Message(
        frame_id=msg['id'],
        name=msg['name'],
        length=msg['length_bits'],
        signals=signals,
        is_extended_frame=msg['extended_id'],
    )
    
    db.messages.append(message)

# Save the database to a DBC file
with open('output.dbc', 'w') as f:
    f.write(db.as_dbc_string())
