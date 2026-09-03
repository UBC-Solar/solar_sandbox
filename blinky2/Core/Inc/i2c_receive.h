#ifndef __I2C_RECEIVE_H__
#define __I2C_RECEIVE_H__

#include "main.h"

#define GPS_ADDR          (0x42 << 1)
#define GPS_MESSAGE_LEN   500
#define GPS_I2C_TIMEOUT   1000

extern volatile uint16_t gps_received_length;
extern volatile HAL_StatusTypeDef gps_config_status;

HAL_StatusTypeDef gps_check_ready(void);
HAL_StatusTypeDef gps_configure_nmea_250ms(void);
HAL_StatusTypeDef read_i2c_gps_module(uint8_t *receive_buffer);

#endif /* __I2C_RECEIVE_H__ */
