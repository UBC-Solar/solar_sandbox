#include "i2c_receive.h"
#include "i2c.h"

HAL_StatusTypeDef read_i2c_gps_module(uint8_t *receive_buffer)
{
  if (!receive_buffer) return HAL_ERROR;

  return HAL_I2C_Master_Receive(&hi2c1, GPS_ADDR, receive_buffer, GPS_MESSAGE_LEN, HAL_MAX_DELAY);
}

