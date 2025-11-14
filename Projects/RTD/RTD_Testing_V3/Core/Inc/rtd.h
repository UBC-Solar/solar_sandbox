/*
 * rtd.h
 *
 *  Created on: Nov 1, 2025
 *      Author: Luke
 */

#ifndef INC_RTD_H_
#define INC_RTD_H_

typedef union {
    struct {
        Bool OUV : 1;                    // bit0 = D2 (Over/undervoltage)
        Bool RTDIN_Low_Voltage : 1;      // bit1 = D3
        Bool REFIN_Low : 1;              // bit2 = D4
        Bool REFIN_High : 1;             // bit3 = D5
        Bool Low_Threshold_Violation : 1;// bit4 = D6
        Bool High_Threshold_Violation : 1;// bit5 = D7
    } faults;
    uint8_t bits;
} Rtd_status_t;


#endif /* INC_RTD_H_ */
