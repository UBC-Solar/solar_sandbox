// Example







// CAN ISR
// Some code that receives CAN msg with HAL

HAL_CAN_rx();

// convert to CAL datatype

CAL_msg_t can_msg;


CAL_parse_rx_data(can_msg);
