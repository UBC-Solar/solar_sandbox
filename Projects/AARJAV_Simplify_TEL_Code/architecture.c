#include <stdint.h>

#define MAX_CAN_DATA_LEN             8

typedef struct CAN_RadioMsg_s {
    double timestamp;
    char ID_DELIMETER;
    uint32_t can_id_reversed;
    uint8_t data[MAX_CAN_DATA_LEN];
    uint8_t data_len;
    char CARRIAGE_RETURN;
    char NEW_LINE;
} CAN_RadioMsg_TypeDef;




#define CONST_UINT32_BYTE_REVERSE(num)    ((((num) & 0x000000FF) << 24) | (((num) & 0x0000FF00) << 8) | (((num) & 0x00FF0000) >> 8) | (((num) & 0xFF000000) >> 24))

#define ID_DELIMITER                '#'
#define CARRIAGE_RETURN_CHAR        '\r'
#define NEW_LINE_CHAR               '\n'

int main(void) {
    CAN_RadioMsg_TypeDef current_CAN_rx = {0};                  // 0 it.

    current_CAN_rx.ID_DELIMETER     = ID_DELIMITER;             // Set all the constant fields
    current_CAN_rx.CARRIAGE_RETURN  = CARRIAGE_RETURN_CHAR;
    current_CAN_rx.NEW_LINE         = NEW_LINE_CHAR;

    while (1) {
        if (<LOGIC TO GET CAN MESSAGE>) {
            check_and_set_rtc(<CAN_DATA>, <CAN_ID>);                        // If 0x300 then RTC sync

            current_CAN_rx.timestamp = get_current_time();
            current_CAN_rx.can_id_reversed = CONST_UINT32_BYTE_REVERSE(<CAN_ID>); // Reverse for readability
            current_CAN_rx.data = <POINTER TO DATA RECIEVED>;               // No need to copy because we Tx in this scope so stack is preserved
            current_CAN_rx.data_len = <LOGIC FOR CAN DATA LEN>;

            flash_can_led();                                                // Pseudo code to flash LED

            while (UART_Tx_DMA_BUSY()) {}                                   // Wait for previous Tx to complete using callback to check
            UART_Tx_DMA();                                                  // Pseudo code to UART tx via DMA
        }
    }
}