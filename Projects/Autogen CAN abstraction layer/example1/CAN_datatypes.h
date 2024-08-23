// CAN_datatypes.h
#include <stdint.h>
#include <bool.h>

typedef void (*can_tx_function_pointer)(uint8_t*, uint8_t); // function pointer to the can_tx_function_pointer declaration. Takes in an array of uint8_t and its length as args

typedef struct
{
    can_tx_function_pointer tx_function;
} CAL_config_t;


// MotorCommand CAN msg
#define MOTORCOMMAND_DATA_LENGTH 5

typedef struct
{
    uint16_t throttle_position; // 16 bits
    uint8_t drive_state; // 8 bits
} MotorCommand_t;


typedef struct
{
    uint8_t data_length; // 8 bits
    uint32_t data_id; // 8 bits
} CAL_msg_t;

