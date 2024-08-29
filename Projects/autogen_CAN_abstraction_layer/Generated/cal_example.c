#include <stdint.h>
#include <stdbool.h>





/** HAL STRUCTS  */
/**
  * @brief  CAN Tx message header structure definition
  */
typedef struct
{
  uint32_t StdId;    /*!< Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */

  uint32_t ExtId;    /*!< Specifies the extended identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */

  uint32_t IDE;      /*!< Specifies the type of identifier for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_identifier_type */

  uint32_t RTR;      /*!< Specifies the type of frame for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_remote_transmission_request */

  uint32_t DLC;      /*!< Specifies the length of the frame that will be transmitted.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

  FunctionalState TransmitGlobalTime; /*!< Specifies whether the timestamp counter value captured on start
                          of frame transmission, is sent in DATA6 and DATA7 replacing pData[6] and pData[7].
                          @note: Time Triggered Communication Mode must be enabled.
                          @note: DLC must be programmed as 8 bytes, in order these 2 bytes are sent.
                          This parameter can be set to ENABLE or DISABLE. */

} CAN_TxHeaderTypeDef;

/**
  * @brief  CAN Rx message header structure definition
  */
typedef struct
{
  uint32_t StdId;    /*!< Specifies the standard identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x7FF. */

  uint32_t ExtId;    /*!< Specifies the extended identifier.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0x1FFFFFFF. */

  uint32_t IDE;      /*!< Specifies the type of identifier for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_identifier_type */

  uint32_t RTR;      /*!< Specifies the type of frame for the message that will be transmitted.
                          This parameter can be a value of @ref CAN_remote_transmission_request */

  uint32_t DLC;      /*!< Specifies the length of the frame that will be transmitted.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 8. */

  uint32_t Timestamp; /*!< Specifies the timestamp counter value captured on start of frame reception.
                          @note: Time Triggered Communication Mode must be enabled.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFFFF. */

  uint32_t FilterMatchIndex; /*!< Specifies the index of matching acceptance filter element.
                          This parameter must be a number between Min_Data = 0 and Max_Data = 0xFF. */

} CAN_RxHeaderTypeDef;
/** END HAL STRUCTS */





/**
 * HEADER IDEA
 * 
 * we will define our own header struct using the CAL notation
 * Then in any function arguments (stuff user has to provide us) we expect them to use our structs
 * However, in the CAL functions implementation (like for CAL_Tx_<SOME_MESSAGE>) we will convert our structs to the actual structs that the CAN library expects
 */
typedef struct {
  uint32_t can_id;
  bool is_extended_id;  
  uint8_t data_length;
} CAL_CAN_header_t



void _convert_cal_to_can_hal_header(CAL_CAN_header_t* cal_header, CAN_TxHeaderTypeDef* hal_header) {
    hal_header->StdId   = cal_header->is_extended_id ? 0U : cal_header->can_id;
    hal_header->ExtId   = cal_header->is_extended_id ? cal_header->can_id : 0U;
    hal_header->IDE     = cal_header->is_extended_id ? CAN_ID_EXT : CAN_ID_STD;
    hal_header->RTR     = CAN_RTR_DATA;
    hal_header->DLC     = cal_header->data_length;
}


void CAL_Tx_voltagesensordata(uint8_t* data) {
    CAN_TxHeaderTypeDef HAL_header;

    // Call the actual CAN library function
    cal_config->CAN_Tx(&HAL_header, data);

}


CAL_CAN_header_t CAL_voltagesensordata_header = {               // Autogened
    .can_id = 0x123,
    .is_extended_id = false,
    .data_length = 8
};


/**
 * This is what the Rx data funtion fills
 * We need the parsed data then the header in case user needs it
 * Plus providing the pointer is not bad beacuse I am not making a whole copy of the header struct just a pointer to something that already exists
 * Wait but if the header already exists then in the Tx function I can just use that header directly
 * Wait but if I have a Tx function dedicated to that specifc message then I dont need to pass in the header I can just use it
 * Actually I will just no even make our own headers. I mean why? I will just autogen the structs for HAL CAN drivers.
 * Nono that goes back on the point of not being HAL CAN oriented. Techinically its better to have our own structs and then convert them to HAL structs
 * to keep abstraction.
 * 
 * IDEA: Keep in the YAML file an option to just turn off the HAL structs and then we will just use the HAL structs directly
 */
typedef struct {                                        
    float voltagesensor1;
    float voltagesensor2;
    CAN_RxHeaderTypeDef* CAL_voltagesensordata_header;
} CAL_voltagesensordata_t;

/**
 * Instead of using a global header and data field as structs
 * And then we set those in this function, we can just use those globals
 * as typedefs and then create structs of that type in this function
 * Then the return type of this function will be a pointer to that specifc message structs data
 * 
 */
#define CAN_MAX_DATA_LENGTH 8
void CAL_Rx_voltagesensordata() {
    CAL_CAN_header_t header;
    uint8_t HAL_data[CAN_MAX_DATA_LENGTH];

    // Call the actual CAN library function
    cal_config->CAN_Rx(&HAL_header, HAL_data);

    // Convert the actual CAN header to our header

}


int main() {
    CAL_Tx_voltagesensordata()

    return 0;
}








// In an Autogened file....
typedef struct {
	uint16_t throttle_adc_reading;
	bool pedal_adc_out_of_range;
	bool regen_enabled;
	bool cruise_control_enabled;
	bool mech_brake_pressed;
	bool mdi_comm_fault;
} CAL_msg_mcb_diagnostics_0x404;


typedef struct {
	float current_sensor1;
	float current_sensor2;
} CAL_msg_amb_current_0x702;


// Yo define CAL_FIXED_POINT if you want to do this


// Instead of the Set and Send style code in the MCB:
CAL_msg_mcb_diagnostics_0x404 diagnostic_msg = {0};

diagnostic_msg.throttle_adc_reading = my_throttle;
diagnostic_msg.pedal_adc_out_of_range = <SOME_LOGIC>;
diagnostic_msg.regen_enabled = <SOME_LOGIC>;
diagnostic_msg.cruise_control_enabled = <SOME_LOGIC>;
diagnostic_msg.mech_brake_pressed = <SOME_LOGIC>;
diagnostic_msg.mdi_comm_fault = <SOME_LOGIC>;

CAL_Tx_mcb_diagnostics_0x404(&diagnostic_msg) {
    // Convert CAL_msg specifc struct to raw NUM BYTES
    // You already know the length
    uint8_t mcb_data[CAL_MCB_DIAGNOSTICS_DLC] = {0};

    // Convert signals to bytes
    SET_BIT     

    CAL_package_bool(mcb_data, diagnostic_msg.pedal_adc_out_of_range, 0, 1);
    CAL_package_uint16(mcb_data, diagnostic_msg.throttle_adc_reading, 1, );


    #ifndef CAL_FIXED_POINT
        do normal float conversion
        
    cal_config->CAN_Tx(&CAL_mcb_diagnostics_0x404_header, mcb_data);
}





// Some Autogened File
typedef struct {
	uint8_t* data;
	uint32_t id;
} CAL_MsgTypeDef;

// In DID main code:
CAL_MsgTypeDef* did_rx_msg = CAL_Rx_GetMessage();

switch(did_rx_msg.id)
{
		case CAL_MSG_MCB_DIAGNOSTICS_ID:
			CAL_msg_mcb_diagnostics_0x404* mcb_diagnostic_msg = CAL_parse_mcb_diagnostics(did_rx_msg.data);

        case CAL_MSG_AMB_CURRENT_ID:
			CAL_msg_amb_current_0x702* amb_current_msg = CAL_parse_amb_current(did_rx_msg.data);
            SET_CYCLIC_DATA( data_array_current, temp_array_current.float_value );
			// Some cyclic data logic (which can actually be a feature in CAL!)
}

1. get CAL data struct
2. Filter by ID
3. call CAL parse function on that ID message    (example hash map to relate ID to )




CAL_MsgTypeDef* CAL_Rx_GetMessage() {

}