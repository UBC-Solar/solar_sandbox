/*
 * rtd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

#include <stdint.h>

// Public function declarations
void RTD_Init(void);
float RTD_GetTemperature(void);
float RTD_GetResistance(void);
uint8_t RTD_ReadFaults(void);

#endif /* INC_RTD_H_ */
