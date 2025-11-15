/*
 * rtd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke Santosham & Martin Wu
 */

#include "rtd.h"
#include <stdint.h>
#include <stdbool.h>

#define COEFF_OF_RESISTANCE_PLAT	0.00385
#define RESISTANCE_AT_0C 			1000.0
#define REFERENCE_RESISTANCE 		4300.0

// Register Addresses
#define CONFIG_REG      0x00
#define RTD_MSB_REG     0x01
#define RTD_LSB_REG     0x02
#define FAULT_STATUS    0x07

// Config Register Bits
#define CONFIG_VBIAS    0x80  // V_BIAS enabled
#define CONFIG_AUTO     0x40  // Auto conversion mode
#define CONFIG_1SHOT    0x20  // 1-shot conversion
#define CONFIG_3WIRE    0x10  // 3-wire RTD
#define CONFIG_FAULTCYC 0x00  // No fault cycle
#define CONFIG_FILT50HZ 0x01  // 50Hz filter


// Private function prototypes
static void RTD_WriteRegister(uint8_t address, uint8_t data);
static uint8_t RTD_ReadRegister(uint8_t address);
static bool RTD_ResistanceToTemp(uint32_t* temp);
static uint32_t RTD_ReadResistance(void);

// Write to a MAX31865 register
static void RTD_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = address | 0x80;  // Set MSB for write
    buffer[1] = data;

    HAL_SPI_Transmit(&hspi1, buffer, 2, 100);
}

// Initialize the MAX31865 chip
void RTD_Init(void) {
	uint8_t config = CONFIG_VBIAS | CONFIG_AUTO | CONFIG_3WIRE | CONFIG_FILT50HZ;
	
	RTD_WriteRegister(CONFIG_REG, config);

}

// Read from a MAX31865 register
static uint8_t RTD_ReadRegister(uint8_t address) {
	uint8_t tx_buffer = address & 0x7F;  // Clear MSB for read
	uint8_t rx_buffer = 0;
	
	HAL_SPI_Transmit(&hspi1, &tx_buffer, 1, 100);
	HAL_SPI_Receive(&hspi1, &rx_buffer, 1, 100);
	
	return rx_buffer;
}

// Read the 15-bit RTD value from MAX31865
uint16_t RTD_ReadRTD(void) {
	uint8_t msb = RTD_ReadRegister(RTD_MSB_REG);
	uint8_t lsb = RTD_ReadRegister(RTD_LSB_REG);
	
	uint16_t rtd = (msb << 8) | lsb;
	
	
	// Shift right by 1 to get the 15-bit RTD value
	rtd >>= 1;
	
	return rtd;
}


static uint32_t RTD_ReadResistance(void) {
	uint16_t rtd_raw = RTD_ReadRTD();
	
	uint32_t resistance = ((uint32_t)rtd_raw * (uint32_t)REFERENCE_RESISTANCE) / 32768;
	
	return resistance;
}


static bool RTD_ResistanceToTemp(uint32_t* temp) {
	uint32_t resistance = RTD_ReadResistance();
	
	int32_t temp_celsius = ((int32_t)resistance - (int32_t)RESISTANCE_AT_0C) / COEFF_OF_RESISTANCE_PLAT;
	
	*temp = (uint32_t)temp_celsius;
	
	return false;
}

Rtd_status_t RtdGetTemperature(uint32_t* temperature) {
	Rtd_status_t status = {false, false};
	uint32_t temp;
	bool fault;
	
	fault = RTD_ResistanceToTemp(&temp);
	
	if(!fault) {
		*temperature = temp;
	}
	
	return status;
}


