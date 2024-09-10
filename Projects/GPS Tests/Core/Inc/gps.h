/*
 * gps.h
 *
 *  Created on: Sep 7, 2024
 *      Author: kcgro
 */

#ifndef GPS_H
#define GPS_H

#include "i2c.h"
#include <stdbool.h>

#define GPS_MESSAGE_LEN 1000

bool read_i2c_gps_module(uint8_t *receive_buffer);

#endif /* GPS_H */
