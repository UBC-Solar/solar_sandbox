"""
The purpose of this script is to turn all struct definitons into typdefs
inside the brightside.h file.

Ex.
struct brightside_fan_speed_measurement_t {
    /**
     * Dummy signal in empty message.
     */
    uint8_t dummy;
};

to

typedef struct {

    /**
     * Dummy signal in empty message.
     */
    uint8_t dummy;

} brightside_fan_speed_measurement_t;
"""

import re
import os


# FILE CONSTANTS
CANTOOLS_H             = "brightside.h"
CANTOOLS_C             = "brightside.c"
