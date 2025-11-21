/*
 * rtd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke Santosham & Martin Wu
 */

#include "rtd.h"

#define COEFF_OF_RESISTANCE_PLAT	0.00385
#define RESISTANCE_AT_0C 			1000
#define REFERENCE_RESISTANCE 		4300
#define TIMEOUT_DELAY				100
#define INIT_FAULT_READ				0x84
#define MAX_ATTEMPTS				400
//15 BIT NUMBERS, discard MSB
#define MAX_FAULT_THRESHOLD			0xFFFF
#define MIN_FAULT_THRESHOLD			0x0000

// Register Addresses
#define CONFIG_REG      		0x00
#define RTD_MSB_REG     		0x01
#define RTD_LSB_REG     		0x02
#define MAX_FAULT_THRESHOLD_MSB 0x03
#define MAX_FAULT_THRESHOLD_LSB 0x04
#define MIN_FAULT_THRESHOLD_MSB 0x05
#define MIN_FAULT_THRESHOLD_LSB 0x06
#define FAULT_STATUS    		0x07

// Config Register Bits
#define CONFIG_VBIAS    0x80  // V_BIAS enabled
#define CONFIG_AUTO     0x40  // Auto conversion mode
#define CONFIG_1SHOT    0x20  // 1-shot conversion
#define CONFIG_3WIRE    0x10  // 3-wire RTD
#define CONFIG_FAULTCYC 0x00  // No fault cycle
#define CONFIG_FILT50HZ 0x01  // 50Hz filter


//PRIVATE FUNCTION PROTOTYPES
static bool RTD_WriteRegister(uint8_t address, uint8_t data);
static bool RTD_ReadRegister(uint8_t address, uint8_t* data);
static bool RTD_RtdFaults(Rtd_faults_t* faults);
static bool RTD_ResistanceToTemp(uint32_t* temp);

//PUBLIC FUNCTIONS
/*
 * @brief:	Gets the temperature and status of the RTD
 * @param:	temperature
 * 			->pre	pointer to an uint32 that will contain
 * 					the temperature.
 * 			->post	the temperature of the motor converted
 * 					from the resistance ratio
 * @returns	status of the RTD as an enumerator.
 */
Rtd_status_t RTD_GetTemperature(uint32_t* temperature, Rtd_faults_t* faults){

	bool rtd_read_flag = 0;
	bool fault_read_flag = 0;
	Rtd_status_t status;

	//get temperature, faults and error flags
	rtd_read_flag = RTD_ResistanceToTemp(temperature);
	fault_read_flag = RTD_RtdFaults(faults);

	//assign the RTD status based off flags
	if (fault_read_flag)
		status = RtdFaultReadAttemptExceded;
	else if (rtd_read_flag)
		status = RtdStatusFault;
	else
		status = RtdStatusOk;

	return status;
}

void RTD_Init(){
	//write initial configuration
	uint8_t config = CONFIG_VBIAS | CONFIG_AUTO | CONFIG_3WIRE | CONFIG_FILT50HZ;
	RTD_WriteRegister(CONFIG_REG, config);

	//write fault thresholds
	uint8_t buffer;

	buffer = (uint8_t)(((uint16_t)MAX_FAULT_THRESHOLD & 0x00FF) << 1);
	RTD_WriteRegister(MAX_FAULT_THRESHOLD_LSB, buffer);
	buffer = (uint8_t)((((uint16_t)MAX_FAULT_THRESHOLD >> 8) & 0x00FF) << 1);
	RTD_WriteRegister(MAX_FAULT_THRESHOLD_MSB, buffer);

	buffer = (0xFF & MIN_FAULT_THRESHOLD) << 1;
	RTD_WriteRegister(MIN_FAULT_THRESHOLD_LSB, buffer);
	buffer = (0xFF & MIN_FAULT_THRESHOLD >> 8) << 1;
	RTD_WriteRegister(MIN_FAULT_THRESHOLD_MSB, buffer);

}

//PRIVATE FUNCTIONS
static bool RTD_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {0};
    buffer[0] = address | 0x80;
    buffer[1] = data;
	bool hal_status_flag = 0;

    hal_status_flag = HAL_OK != HAL_SPI_Transmit(&hspi1, buffer, 2, TIMEOUT_DELAY);
	return hal_status_flag;
}

static bool RTD_ReadRegister(uint8_t address, uint8_t* data){
	uint8_t read_addr = address & 0x7F;
	bool hal_status_flag = 0;

	hal_status_flag = HAL_OK != HAL_SPI_Transmit(&hspi1, &read_addr, 1, TIMEOUT_DELAY);
	hal_status_flag = HAL_OK != HAL_SPI_Receive(&hspi1, data, 1, TIMEOUT_DELAY);
	return hal_status_flag;
}

static bool RTD_RtdFaults(Rtd_faults_t* faults){
	uint8_t raw = 0;
	bool max_attempt_flag = 0;
	int i = 0;

	//saves the current state of the config register
	uint8_t prev;
	RTD_ReadRegister(CONFIG_REG, &prev);
	uint8_t curr;
	uint8_t init_fault = (prev & 0x11) | INIT_FAULT_READ;

	//initiate an automatic fault read
	RTD_WriteRegister(CONFIG_REG, init_fault);

	//exits fault read when {D2,D3} of config register is 00b
	//for loop is implemented so the code cannot get stuck in a while loop
	for (i = 0; i < MAX_ATTEMPTS; i++){
		RTD_ReadRegister(CONFIG_REG, &curr);

		//if a cycle completes, restore the previous configuration
		if(((curr >> 2) ^ 1) && ((curr >> 3) ^ 1)){
			RTD_WriteRegister(CONFIG_REG, prev);
			break;
		}
	}

	//if the loop exceeds the max attempts, set the error flag
	if (i >= MAX_ATTEMPTS) max_attempt_flag = true;

	//read the fault register after an auto fault detection cycle has occurred
	RTD_ReadRegister(FAULT_STATUS, &raw);
	faults->bits = raw >> 2;

	return max_attempt_flag;
}

static bool RTD_ResistanceToTemp(uint32_t* temp){

	uint32_t resistance;
	uint16_t buffer;
	uint8_t msb, lsb;
	bool fault_flag;

	//get the MSB of the ratio
	RTD_ReadRegister(RTD_MSB_REG, &msb);
	//get the LSB of the ratio
	RTD_ReadRegister(RTD_LSB_REG, &lsb);
	buffer = ((uint16_t)msb << 8) | lsb;

	//Fault detection
	fault_flag = buffer & 1;

	//get the fault flag and resistance of the RTD
	resistance = buffer >> 1;
	resistance *= REFERENCE_RESISTANCE;

	*temp = ((uint32_t)resistance - RESISTANCE_AT_0C) / COEFF_OF_RESISTANCE_PLAT / RESISTANCE_AT_0C;

	return fault_flag;
}


