/*
 * rtd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_
#include <stdbool.h>

bool RTD_GetTemperature(uint32_t* temperature, Rtd_faults_t* status);
void RTD_WriteRegister(uint8_t address, uint8_t data);
void RTD_Init();

typedef union {
    struct {
        bool OUV : 1;                    // bit0 = D2 (Over/undervoltage)
        bool RTDIN_Low_Voltage : 1;      // bit1 = D3
        bool REFIN_Low : 1;              // bit2 = D4
        bool REFIN_High : 1;             // bit3 = D5
        bool Low_Threshold_Violation : 1;// bit4 = D6
        bool High_Threshold_Violation : 1;// bit5 = D7
    } faults;
    uint8_t bits;
} Rtd_faults_t;


#endif /* INC_RTD_H_ */
