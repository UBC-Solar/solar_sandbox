/*
 * rtd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke Santosham
 */

#define COEFF_OF_RESISTANCE_PLAT	0.00385
#define RESISTANCE_AT_0C 			1000
#define REFERENCE_RESISTANCE 		4300

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
Rtd_status_t RtdGetTemperature(Uint32* temperature){

	Uint32 temp;
	Bool fault;
	fault = RTD_ResistanceToTemp(&temp);

	if(!fault){
		*temperature = temp;

	}



}
void RTD_Init(void);
void RTD_WriteRegister(uint8_t address, uint8_t data);


void RTD_WriteRegister(uint8_t address, uint8_t data) {
    uint8_t buffer[2];
    buffer[0] = address | 0x80;
    buffer[1] = data;

    HAL_SPI_Transmit(&hspi1, buffer, 2, 100);
}

void RTD_Init(void){
	uint8_t config = CONFIG_VBIAS | CONFIG_AUTO | CONFIG_3WIRE | CONFIG_FILT50HZ;
	
	RTD_WriteRegister(CONFIG_REG, config);	
}

uint8_t RTD_ReadRegister(uint8_t address){
	uint8_t tx_buffer = address & 0x7F; //Transmit buffer
	uint8_t rx_buffer = 0; //Receive Buffer
	
	HAL_SPI_TransmitReceive(&hspi1, tx_buffer, rx_buffer, 1, 100);
	
	return rx_buffer;
}

uint16_t RTD_ReadRTD (void){
	uint8_t msb = RTD_ReadRegister(RTD_MSB_REG);
	uint8_t lsb = RTD_ReadRegister(RTD_LSB_REG);
	
	uint16_t rtd = (msb << 8) | lsb;
	rtd >>= 1;
	
	return rtd;
}



// private
Bool RtdWriteConfig(tbd);
Rtd_status_t GetRtdFaults();
Uint32 RTD_ReadResistance();

Bool RTD_ResistanceToTemp(Uint32* temp){

	Uint32 resistance;
	Bool fault;
	fault = RTD_ReadResistance(&resistance);

	if (!fault)
		*temp = resistance * COEFF_OF_RESISTANCE_PLAT + RESISTANCE_AT_0C;

	return fault;
}


