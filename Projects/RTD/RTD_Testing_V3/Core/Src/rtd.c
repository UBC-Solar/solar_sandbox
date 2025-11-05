/*
 * rtd.c
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke Santosham
 */

#define COEFF_OF_RESISTANCE_PLAT	0.00385
#define RESISTANCE_AT_0C 			1000
#define REFERENCE_RESISTANCE 		4300

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
void RtdInit();

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


