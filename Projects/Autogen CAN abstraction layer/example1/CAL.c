// CAL.c
#include <stdint.h>
#include <CAN_datatypes.h>

can_tx_function_pointer tx_function = NULL;

// Set tx function pointer
uint8_t initialize_CAL(CAL_config_t config)
{
    tx_function = config.tx_function;

    return 0;
}

uint8_t CAL_tx_MotorCommand(MotorCommand_t MotorCommand)
{
    uint8_t tx_data[MOTORCOMMAND_DATA_LENGTH];
    // Package MotorCommand struct into tx_data array
    // ...

    tx_function(tx_data, MOTORCOMMAND_DATA_LENGTH);
}

MotorCommand_t g_MotorCommand;

uint8_t CAL_parse_rx_data(CAL_msg_t* CAL_msg)
{
    switch(CAL_msg.data_id)
    {
        case MOTOR_COMMAND_DATA_ID
            CAN_parse_MotorCommand(&g_MotorCommand);

        // More cases
    }
}

uint8_t CAL_get_MotorCommand(MotorCommand_t* MotorCommand)
{
    memset(MotorCommand, g_MotorCommand);
    return error;
}