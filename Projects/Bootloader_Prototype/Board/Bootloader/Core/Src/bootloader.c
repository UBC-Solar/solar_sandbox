/**
 *  @file bootloader.c
 *  @brief Defines bootloader functions used for jumping to main application, and flashing new main application
 *
 *  @date 2024/08/24
 *  @author Evan Owens
 */

#include "bootloader.h"
#include "main.h"

//BOOTLOADER GLOBAL VARIABLES
//FLASH user app into flash sector 5 (but any sector other than one works)
uint32_t memory_address = FLASH_SECTOR5_BASE_ADDRESS; //changes to track address to flash new chunks to
extern UART_HandleTypeDef huart2;

//BOOTLOADER FLASHING FUNCTIONS

/**
 * @brief Handles flashing of new application by clearing memory, sending an ack,
 * and stepping into a while loop that flashes application by looping until all data has been receieved,
 * while writing each chunk of data into memory, and sending an ack when its successfully done.
 * @param NONE
 * @return void
*/

void bootloader_flash_application(void){

	uint8_t flash_status = HAL_OK;

	if (bootloader_flash_erase(SECTOR_5_FLASH, SINGLE_SECTOR) == HAL_OK){
		if (bootloader_send_ack() == HAL_OK){
			while (flash_status == HAL_OK){

				flash_status = bootloader_flash_chunk();

				if (flash_status== HAL_OK){
					bootloader_send_ack();
				}
				else if (flash_status == FLASH_COMPLETE){
					bootloader_load_user_application();
				}
				else
				{
					bootloader_send_nack();
				}
			}
		}
	}
}

/**
 * @brief Handles flashing of each individual chunk of data
 * @param None
 * @return status: uint8_t value indicating either:
 * 		a. HAL_OK when chunk has successfully been receieved and written and we are ready to receive another chunk
 * 		b. HAL_ERROR when chunk either does not successfully get receievd or there is an error in the flashing process
 * 		c. FLASH_COMPLETE when chunk received has specific bytes indicating entire program has been sent and written
*/

uint8_t bootloader_flash_chunk(void){

	uint8_t status;
	uint8_t decider_buffer[PACKET_OVERHEAD_LENGTH];

	//receieve first two bytes of chunk to check if there is more data to receive, and if so, how long it is.
	HAL_UART_Receive(&huart2, decider_buffer, sizeof(decider_buffer), HAL_MAX_DELAY);

	if (decider_buffer[PACKET_STATUS_BYTE_POSITION] == ALL_DATA_SENT)
	{
		status = FLASH_COMPLETE;
	}
	else if (decider_buffer[PACKET_STATUS_BYTE_POSITION] == MORE_DATA_INCOMING)
	{
		//create an array the size of the data chunk based off second byte in packet
		uint8_t data_buffer[decider_buffer[PACKET_DATA_LENGTH_POSITION]];

		HAL_UART_Receive(&huart2, data_buffer, sizeof(data_buffer), HAL_MAX_DELAY);
		status = bootloader_flash_memory(data_buffer, memory_address, sizeof(data_buffer));

		//update memory address so next chunk gets written after previous chunk
		memory_address += sizeof(data_buffer);
	}
	else
	{
		status = HAL_ERROR;
	}

	return status;
}



/**
 * @brief Handles flashing of each chunk of memory by looping through chunk and writing each individual
 * byte to memory (using HAL_FLASH_PROGRAM function)
 * @param data[]: byte array of all the bytes in a chunk that are getting written to flash
 * @param start_address: start position of where current chunk is getting written to flash
 * @param data_size: size of chunk of data
 * @return status: HAL_StatusTypeDef typecasted to uint8_t variable that either returns:
 * 		a.HAL_OK if each byte has been successfully been written to memory
 * 		b. HAL_ERROR/HAL_BUSY not every byte has been successfully been written to memory
*/

uint8_t bootloader_flash_memory(uint8_t data[], uint32_t start_address, uint16_t data_size){

	HAL_StatusTypeDef status = HAL_OK;
	uint32_t address = start_address;

	HAL_FLASH_Unlock();

	for (int i =0; i < data_size; i++){

		status = (uint8_t)HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, address+i, data[i]);

		if (status != HAL_OK){
			break;
		}
	}

	HAL_FLASH_Lock();
	return status;
}



//BOOTLOADER ERASING FUNCTIONS

/**
 * @brief Handles erasing of sectors of memory to allow them to be flashed
 * @param sector_number:the sector that the erasing is to start
 * @param num_sectors: the number of sectors that are being erased
 * @return status: HAL_StatusTypeDef typecasted to uint8_t variable that either returns:
 * 		a.HAL_OK if erase has been successful
 * 		b. HAL_ERROR if erase has been unsuccessful
*/

uint8_t bootloader_flash_erase(uint8_t sector_number, uint8_t num_sectors){

	FLASH_EraseInitTypeDef flashEraseHandle;
	uint32_t sectorError;
	HAL_StatusTypeDef status;

	flashEraseHandle.TypeErase = FLASH_TYPEERASE_SECTORS;
	flashEraseHandle.Sector = sector_number;
	flashEraseHandle.NbSectors = num_sectors;
	flashEraseHandle.Banks = FLASH_BANK_1;

	HAL_FLASH_Unlock();

	flashEraseHandle.VoltageRange = FLASH_VOLTAGE_RANGE_3; //mcu specific?
	status = (uint8_t) HAL_FLASHEx_Erase(&flashEraseHandle, &sectorError);

	HAL_FLASH_Lock();

	return status;
}




//BOOTLOADER load user application functions

/**
 * @brief Creates a function pointer to reset handler of user application, dereferences it, and calls it,
 * causing board to jump to user application
 * @param NONE
 * @return VOID

*/

void bootloader_load_user_application(void){

	void (*app_reset_handler)(void);
	volatile uint32_t msp_value = *(volatile uint32_t *)0x8020000;

	app_reset_handler  = (void (*) (void)) (*((uint32_t *) 0x8020004));

	__set_MSP(msp_value);

	app_reset_handler();
}



//BOOTLOADER UART functions

/**
 * @brief Sends acknowledgement to host, prompting host to start sending chunks
 * @param NONE
 * @return status of UART transmit (either HAL_OK or HAL_ERROR)
*/

uint8_t bootloader_send_ack(void){
	HAL_StatusTypeDef status;
	uint8_t startup_ack[1] = {0x01};
	status = (uint8_t)HAL_UART_Transmit(&huart2, startup_ack, sizeof(startup_ack), HAL_MAX_DELAY);

	return status;
}



/**
 * @brief Sends negative acknowledgement to host in case of error, ending flashing process
 * @param NONE
 * @return status of UART transmit (either HAL_OK or HAL_ERROR)
*/
uint8_t bootloader_send_nack(void){
	HAL_StatusTypeDef status;
	uint8_t startup_nack[1] = {0x00};
	status = (uint8_t)HAL_UART_Transmit(&huart2, startup_nack, sizeof(startup_nack), HAL_MAX_DELAY);

	return status;
}

