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
#define INIT_FAULT_READ				0x84
#define MAX_ATTEMPTS				400

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
bool RTD_GetTemperature(uint32_t* temperature, Rtd_faults_t* faults){

	bool status;

	status = !RTD_ResistanceToTemp(temperature);	//inverted so 1 == success, 0 == fail
	*faults = RTD_ReadFaults();

	return status;
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

Rtd_faults_t RTD_RtdFaults(){
	uint8_t raw = 0;
	Rtd_faults_t faults = {0};
	int i = 0;

	//saves the current state of
	uint8_t prev = RTD_ReadRegister(CONFIG_REG);
	uint8_t curr;
	uint8_t init_fault = (prev & 0x11) | INIT_FAULT_READ;

	RTD_WriteRegister(CONFIG_REG, init_fault);

	for (i = 0; i++; i < MAX_ATTEMPTS){
		curr = RTD_ReadRegister(CONFIG_REG);

		if((curr >> 2) ^ 1 && (curr >> 3) ^ 1){
			RTD_WriteRegister(CONFIG_REG, prev);
			break;
		}
	}

	raw = RTD_ReadRegister(FAULT_STATUS);
	status.bits = raw >> 2;

	return status;
}

bool RTD_ReadResistanceRatio(uint32_t* resistance_ratio){

	uint16_t buffer;
	bool     status;

	//get the MSB of the ratio
	buffer = RTD_ReadRegister(RTD_MSB_REG);
	buffer <<= 8;
	//get the LSB of the ratio
	buffer = RTD_ReadRegister(RTD_LSB_REG);

	//Fault detection
	status = buffer & 1;

	*resistance_ratio = buffer >> 1;
	return status;

}

bool RTD_ResistanceToTemp(uint32_t* temp){

	uint32_t resistance;
	bool status;

	//get the fault and resistance of the RTD
	status = RTD_ReadResistanceRatio(&resistance);
	resistance *= REFERENCE_RESISTANCE;

	if (!status)
		*temp = resistance * COEFF_OF_RESISTANCE_PLAT + RESISTANCE_AT_0C;

	return status;
}


