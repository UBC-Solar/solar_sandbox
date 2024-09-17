/*
 * gps.c
 *
 *  Created on: Sep 7, 2024
 *      Author: kcgro
 */

#include "gps.h"

#define GPS_DEVICE_ADDRESS ((0x42)<<1)
#define GPS_I2C_TIMEOUT    (1000)

/**
 * @brief Continually tries to get a fix. Only returns once a fix is obtained
 * @param receive_buffer The buffer to store the received data
 * @return bool True if a fix was obtained, false otherwise
 */
bool read_i2c_gps_module(uint8_t* receive_buffer)
{
	bool status = false;
    if(HAL_I2C_Master_Receive_IT(&hi2c1, GPS_DEVICE_ADDRESS, receive_buffer, GPS_MESSAGE_LEN) == HAL_OK)
    {
        // Set status to true if i2c read was successful
        status = true;
    }

    return status;
}
