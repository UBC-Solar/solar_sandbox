/*
 * rtd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke Santosham & Martin Wu
 */

#define COEFF_OF_RESISTANCE_PLAT	0.00385
#define RESISTANCE_AT_0C 			1000
#define REFERENCE_RESISTANCE 		4300
#define TIMEOUT_DELAY				100

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

//PUBLIC FUNCTIONS
/*
 * PURPOSE:	converts the resistance ratio into temperature and
 * 			writes it to the temperature pointer. Status is a
 * 			fault bit copied from the resistance register on
 * 			the MAX31865 chip.
 * PRE:		temperature: pointer to an Uint32 that will contain
 * 			the temperature
 * POST:	temperature: the temperature of the motor converted
 * 			from the resistance ratio
 * RETURN:	A fault bit copied from the resistance register on
 * 			the MAX31865 chip
 */
Bool RTD_GetTemperature(uint32_t* temperature, Rtd_status_t* status){

	Bool fault;

	fault = RTD_ResistanceToTemp(temperature);
	*status = RTD_ReadFaults();

	return fault;
}


void RTD_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = address | 0x80;
    buffer[1] = data;

    HAL_SPI_Transmit(&hspi1, buffer, 2, TIMEOUT_DELAY);
}


void RTD_Init(){
	uint8_t config = CONFIG_VBIAS | CONFIG_AUTO | CONFIG_3WIRE | CONFIG_FILT50HZ;

	RTD_WriteRegister(CONFIG_REG, config);

	//We also need to initiate high and low thresholds for fault detection
}

//PRIVATE FUNCTIONS

uint8_t RTD_ReadRegister(uint8_t address){
	uint8_t data = 0;
	uint8_t read_addr = address | 0x80;

	HAL_SPI_Transmit(&hspi1, &read_addr, 1, TIMEOUT_DELAY);
	HAL_SPI_Recieve(&hspi1, &data, 1, TIMEOUT_DELAY);

	return data;
}

Rtd_status_t RTD_RtdFaults(){
	uint8_t raw = 0;
	Rtd_status_t status = {0};

	raw = RTD_ReadRegister(FAULT_STATUS);
	status.bits = raw >> 2;

	return status;
}

Bool RTD_ReadResistanceRatio(uint32_t* resistance_ratio){

	uint16_t buffer;
	Bool     fault;

	//get the MSB of the ratio
	buffer = RTD_ReadRegister(RTD_MSB_REG);
	buffer <<= 8;
	//get the LSB of the ratio
	buffer = RTD_ReadRegister(RTD_LSB_REG);

	//Fault detection
	fault = buffer & 1;

	*resistance_ratio = buffer >> 1;
	return fault;

}

Bool RTD_ResistanceToTemp(uint32_t* temp){

	uint32_t resistance;
	Bool fault;

	//get the fault and resistance of the RTD
	fault = RTD_ReadResistanceRatio(&resistance);
	resistance *= REFERENCE_RESISTANCE;

	if (!fault)
		*temp = resistance * COEFF_OF_RESISTANCE_PLAT + RESISTANCE_AT_0C;

	return fault;
}


