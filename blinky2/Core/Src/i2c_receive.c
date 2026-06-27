#include "i2c_receive.h"
#include "i2c.h"

volatile uint16_t gps_received_length = 0;
volatile HAL_StatusTypeDef gps_config_status = HAL_ERROR;

static void ubx_checksum(const uint8_t *data, uint16_t length, uint8_t *ck_a, uint8_t *ck_b)
{
  *ck_a = 0;
  *ck_b = 0;

  for (uint16_t i = 0; i < length; i++)
  {
    *ck_a += data[i];
    *ck_b += *ck_a;
  }
}

HAL_StatusTypeDef gps_check_ready(void)
{
  return HAL_I2C_IsDeviceReady(&hi2c1, GPS_ADDR, 1, GPS_I2C_TIMEOUT);
}

HAL_StatusTypeDef gps_configure_nmea_250ms(void)
{
  uint8_t frame[] = {
    0xB5, 0x62,             /* UBX sync */
    0x06, 0x8A,             /* CFG-VALSET */
    0x14, 0x00,             /* 20-byte payload */
    0x00, 0x01, 0x00, 0x00, /* version 0, RAM layer */

    0x02, 0x00, 0x72, 0x10, 0x01,       /* CFG-I2COUTPROT-NMEA = true */
    0xBA, 0x00, 0x91, 0x20, 0x01,       /* CFG-MSGOUT-NMEA_ID_GGA_I2C = 1 */
    0x01, 0x00, 0x21, 0x30, 0xFA, 0x00, /* CFG-RATE-MEAS = 250 ms */

    0x00, 0x00              /* checksum filled below */
  };

  ubx_checksum(&frame[2], sizeof(frame) - 4, &frame[sizeof(frame) - 2], &frame[sizeof(frame) - 1]);

  gps_config_status = HAL_I2C_Master_Transmit(&hi2c1,
                                               GPS_ADDR,
                                               frame,
                                               sizeof(frame),
                                               GPS_I2C_TIMEOUT);
  return gps_config_status;
}

HAL_StatusTypeDef read_i2c_gps_module(uint8_t *receive_buffer)
{
  if (!receive_buffer) return HAL_ERROR;

  gps_received_length = 0;
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1,
                                                    GPS_ADDR,
                                                    receive_buffer,
                                                    GPS_MESSAGE_LEN,
                                                    GPS_I2C_TIMEOUT);
  if (status == HAL_OK)
  {
    gps_received_length = GPS_MESSAGE_LEN;
  }

  return status;
}
