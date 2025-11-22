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
 * @brief:		Gets the temperature and status of the RTD
 * @param[out]:	temperature; a unint32_t of the temperature read from the RTD
 * 				converted from the resistance ratio.
 * @param[out]:	faults; the bits seven fault bits read from the fault register
 * 				of the MAX31856 chip.
 * @returns		status of the RTD as an enumerator.
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

/*
 * @brief:      Initializes the MAX31856 RTD interface and configures fault thresholds.
 * @details:    Writes the desired configuration settings (bias, conversion mode,
 *              wiring type, and filter selection) to the configuration register.
 *              Also programs the maximum and minimum fault‐detection thresholds.
 * @param:      None.
 * @returns:    None.
 */
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
/*
 * @brief:      Writes a single byte to an RTD register over SPI.
 * @param[in]:  address; the 7-bit register address to write to. The MSB is
 *              automatically OR’d with 0x80 to enable write mode.
 * @param[in]:  data; the 8-bit value to write to the selected register.
 * @returns:    true if an SPI error occurred, false on success.
 */
static bool RTD_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {0};
    buffer[0] = address | 0x80;
    buffer[1] = data;
	bool hal_status_flag = 0;

    hal_status_flag = HAL_OK != HAL_SPI_Transmit(&hspi1, buffer, 2, TIMEOUT_DELAY);
	return hal_status_flag;
}

/*
 * @brief:      Reads a single byte from an RTD register over SPI.
 * @param[in]:  address; the 7-bit register address to read from. The MSB is cleared
 *              to ensure read mode.
 * @param[out]: data; a pointer to store the received register value.
 * @returns:    true if an SPI error occurred during transmit or receive,
 *              false on success.
 */
static bool RTD_ReadRegister(uint8_t address, uint8_t* data){
	uint8_t read_addr = address & 0x7F;
	bool hal_status_flag = 0;

	hal_status_flag |= HAL_OK != HAL_SPI_Transmit(&hspi1, &read_addr, 1, TIMEOUT_DELAY);
	hal_status_flag |= HAL_OK != HAL_SPI_Receive(&hspi1, data, 1, TIMEOUT_DELAY);
	return hal_status_flag;
}

/*
 * @brief:      Triggers an automatic fault-detection cycle and retrieves RTD fault bits.
 * @details:    Temporarily modifies the configuration register to issue an
 *              auto-fault-read command. The function repeatedly polls the
 *              configuration register until the fault-read cycle completes or a
 *              timeout occurs. The resulting 7 fault bits from the MAX31856 are
 *              extracted and returned through the faults structure.
 * @param[out]: faults; a pointer to the Rtd_faults_t structure where the parsed
 *              fault bits will be stored.
 * @returns:    true if the fault-detection cycle exceeded MAX_ATTEMPTS (timeout),
 *              false otherwise.
 */
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
		if (((curr & 0x0C) == 0)){
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

/*
 * @brief:      Reads the RTD resistance ratio, converts it into a temperature, and
 *              detects conversion faults.
 * @details:    Retrieves the 15-bit resistance ratio measurement, masks out the
 *              fault flag, and scales the remaining bits by the reference
 *              resistance. The temperature is computed using a simplified linear
 *              PT1000 approximation.
 * @param[out]: temp; pointer to a uint32_t where the converted temperature is stored.
 * @returns:    true if the measurement contains a fault flag, false if the reading
 *              is valid.
 */
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
	fault_flag = buffer & 0x01;

	//get the fault flag and resistance of the RTD
	resistance = buffer >> 1;
	resistance *= REFERENCE_RESISTANCE;

	*temp = (uint32_t)((resistance - RESISTANCE_AT_0C) / (COEFF_OF_RESISTANCE_PLAT * RESISTANCE_AT_0C));

	return fault_flag;
}


