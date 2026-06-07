#ifndef __I2C_RECEIVE_H__
#define __I2C_RECEIVE_H__

#include "main.h"

#define GPS_ADDR          (0x42 << 1)
#define GPS_MESSAGE_LEN   500

HAL_StatusTypeDef read_i2c_gps_module(uint8_t *receive_buffer);

#endif /* __I2C_RECEIVE_H__ */
