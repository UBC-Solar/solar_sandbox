import yaml
import cantools
from cantools.database.can import Signal, Message
from cantools.database.conversion import LinearIntegerConversion
from ParseYaml import YAML_read


def create_signals(signal_data: list) -> list:
    """Create a list of Signal objects from the YAML signal data."""
    signals = []
    for sig in signal_data:
        signal = Signal(
            name=sig['name'],
            start=sig['start_bit'],
            length=sig['signal_bit_length'],
            byte_order=sig['byte_order'],
            is_signed=sig['is_signed'],
            conversion=LinearIntegerConversion(sig['factor'], sig['offset']),
            minimum=sig.get('min'),
            maximum=sig.get('max'),
            unit=sig.get('unit'),
            is_multiplexer=sig.get('is_multiplexer', False),
            multiplexer_ids=sig.get('multiplexer_ids'),
            multiplexer_signal=sig.get('multiplexer_signal')
        )
        signals.append(signal)
    return signals


def create_messages(can_dict: dict) -> list:
    """Create a list of Message objects from the YAML CAN message data."""
    messages = []
    for msg in can_dict['CAN_Messages']:
        signals = create_signals(msg['signals'])
        message = Message(
            frame_id=msg['id'],
            name=msg['name'],
            length=msg['msg_bit_length'],
            signals=signals,
            is_extended_frame=msg['is_extended_frame'],
        )
        messages.append(message)
    return messages


def add_messages_to_db(db, messages: list):
    """Add a list of Message objects to the cantools database."""
    for message in messages:
        db.messages.append(message)


def save_dbc_file(db, file_name: str):
    """Save the cantools database object to a DBC file."""
    with open(file_name, 'w') as f:
        f.write(db.as_dbc_string())


def main():
    # Main process
    can_dict = YAML_read()

    # Create a CAN database
    db = cantools.db.Database()

    # Create and add messages to the database
    messages = create_messages(can_dict)
    add_messages_to_db(db, messages)

    # Save the database to a DBC file
    save_dbc_file(db, 'output.dbc')


if __name__ == '__main__':
    main()


# """
# Purpose: Extract DBC information from CAN Msg YAML and generate *.dbc 
#     Note: We use a dictionary as the format to communicate information.
# Author: Aarjav Jain
# Date: 2024-08-29
# """


# import yaml
# import cantools
# from cantools.database.can import Signal, Message
# from cantools.database.conversion import LinearIntegerConversion
# from ParseYaml import YAML_read

# can_dict = YAML_read()

# # Create a CAN database
# db = cantools.db.Database()

# # Add messages to the database
# for msg in can_dict['CAN_Messages']:
#     signals = []
#     for sig in msg['signals']:
#         signal = Signal(
#             name=sig['name'],
#             start=sig['start_bit'],
#             length=sig['signal_bit_length'],
#             byte_order=sig['byte_order'],
#             is_signed=sig['is_signed'],
#             conversion=LinearIntegerConversion(sig['factor'], sig['offset']),
#             minimum=sig['min'],
#             maximum=sig['max'],
#             unit=sig['unit']
#             is_multiplexer: bool = False,
#             multiplexer_ids: Optional[List[int]] = None,
#             multiplexer_signal: Optional[str] = None,
#         )
#         signals.append(signal)
    
#     message = Message(
#         frame_id=msg['id'],
#         name=msg['name'],
#         length=msg['msg_bit_length'],
#         signals=signals,
#         is_extended_frame=msg['is_extended_frame'],
#     )
    
#     db.messages.append(message)

# # Save the database to a DBC file
# with open('output.dbc', 'w') as f:
#     f.write(db.as_dbc_string())



# import cantools
# from cantools.database import Database
# from cantools.database.can import Message, Signal

# # Create a CAN database
# db = Database()

# # Create the multiplexing signal
# multiplexing_bits = Signal(
#     name='MultiplexingBits',
#     start=0,
#     length=3,
#     byte_order='big_endian',
#     is_signed=False,
#     is_multiplexer=True
# )

# # Create the multiplexed signals for each multiplexer value
# module1 = Signal(
#     name='Module1',
#     start=8,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[0]  # m0
# )

# module2 = Signal(
#     name='Module2',
#     start=16,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[0]  # m0
# )

# module3 = Signal(
#     name='Module3',
#     start=24,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[0]  # m0
# )

# module4 = Signal(
#     name='Module4',
#     start=32,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[0]  # m0
# )

# module5 = Signal(
#     name='Module5',
#     start=8,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[1]  # m1
# )

# module6 = Signal(
#     name='Module6',
#     start=16,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[1]  # m1
# )

# module7 = Signal(
#     name='Module7',
#     start=24,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[1]  # m1
# )

# module8 = Signal(
#     name='Module8',
#     start=32,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[1]  # m1
# )

# module9 = Signal(
#     name='Module9',
#     start=8,
#     length=8,
#     byte_order='big_endian',
#     is_signed=False,
#     multiplexer_ids=[2]  # m2
# )

# # Create the message with multiplexing
# message = Message(
#     frame_id=1576,
#     name='ModuleStatuses',
#     length=5,
#     signals=[
#         multiplexing_bits,
#         module1,
#         module2,
#         module3,
#         module4,
#         module5,
#         module6,
#         module7,
#         module8,
#         module9
#     ],
#     comment='',
#     senders=['BMS'],
#     is_extended_frame=False
# )

# # Add the message to the database
# db.messages.append(message)

# # Optionally, save the database to a DBC file
# with open('ModuleStatuses.dbc', 'w') as f:
#     f.write(db.as_dbc_string())
