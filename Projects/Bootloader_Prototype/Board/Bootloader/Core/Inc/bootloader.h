
/**
 *  @file bootloader.h
 *  @brief header file for bootloader.c
 *
 *  @date 2024/08/24
 *  @author Evan Owens
 *   */


#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

#include <stdint.h>


//Bootloader Defines
#define FLASH_SECTOR5_BASE_ADDRESS 0x08020000U
#define RESET_HANDLER_OFFSET 4


#define JUMP_BYTE 0x01
#define FLASH_BYTE 0x02
#define FLASH_COMPLETE 0x03
#define ALL_DATA_SENT 0x04
#define MORE_DATA_INCOMING 0x05


#define SECTOR_5_FLASH 5
#define SINGLE_SECTOR 1

#define PACKET_STATUS_BYTE_POSITION 0
#define PACKET_DATA_LENGTH_POSITION 1
#define PACKET_OVERHEAD_LENGTH 2

//Bootloader flashing function prototypes
void bootloader_flash_application(void);
uint8_t bootloader_flash_chunk(void);
uint8_t bootloader_flash_memory(uint8_t data[], uint32_t start_address, uint16_t data_size);

//Bootloader uart functions
uint8_t bootloader_send_ack(void);
uint8_t bootloader_send_nack(void);

//Bootloader program load function prototypes
void bootloader_load_user_application(void);


//Bootloader memory erase funcion prototypes
uint8_t bootloader_flash_erase(uint8_t sector_number, uint8_t num_sectors);




#endif /* INC_BOOTLOADER_H_ */
