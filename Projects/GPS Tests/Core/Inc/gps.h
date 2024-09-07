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

bool GPS_wait_for_fix();

#endif /* GPS_H */
