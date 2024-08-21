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