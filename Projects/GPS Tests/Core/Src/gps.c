/*
 * gps.c
 *
 *  Created on: Sep 7, 2024
 *      Author: kcgro
 */

#include "gps.h"

#define GPS_DEVICE_ADDRESS ((0x42)<<1)

/**
 * @brief Continually tries to get a fix. Only returns once a fix is obtained
 * @return void
*/
bool GPS_wait_for_fix()
{
	bool status = false;
    if(HAL_I2C_IsDeviceReady(&hi2c1, GPS_DEVICE_ADDRESS, 1, 1000) == HAL_OK)
    {
    	status = true;
    }

    return status;
}
