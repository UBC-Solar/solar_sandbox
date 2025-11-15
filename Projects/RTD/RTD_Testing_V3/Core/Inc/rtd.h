/*
 * rtd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

#include <stdint.h>
#include <stdbool.h>
#include "main.h"

typedef struct {
	bool High_Threshold_Violation;
	bool Low_Threshold_Violation;
} Rtd_status_t;

// Function declarations
void RTD_Init(void);
Rtd_status_t RtdGetTemperature(uint32_t* temperature);
uint16_t RTD_ReadRTD(void);

#endif /* INC_RTD_H_ */
