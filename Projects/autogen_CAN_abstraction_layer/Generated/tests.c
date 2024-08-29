#include "brightside.h"
#include <stdio.h>


void array_print(uint8_t *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        printf("%x ", array[i]);
    }
    printf("\n");
}

int main(void)
{   
    struct brightside_voltage_sensors_data_t unpacked_data = {0};     
    struct brightside_voltage_sensors_data_t unpacked_data2 = {0};     
    uint8_t packed_data[BRIGHTSIDE_VOLTAGE_SENSORS_DATA_LENGTH];
    int status = -1;

    // init the data
    status = brightside_voltage_sensors_data_init(&unpacked_data);
    printf("brightside_voltage_sensors_data_init: %d\n", status);

    // Pack some data.
    unpacked_data.volt_sensor1 = 100;         // 3.3V 
    unpacked_data.volt_sensor2 = 1.0;         // 3.3V 
    array_print(&packed_data[0], BRIGHTSIDE_VOLTAGE_SENSORS_DATA_LENGTH);
    status = brightside_voltage_sensors_data_pack(&packed_data[0], &unpacked_data, BRIGHTSIDE_VOLTAGE_SENSORS_DATA_LENGTH);
    array_print(&packed_data[0], BRIGHTSIDE_VOLTAGE_SENSORS_DATA_LENGTH);
    printf("brightside_voltage_sensors_data_init: %d\n", status);

    // unpack the data
    status = brightside_voltage_sensors_data_unpack(&unpacked_data2, &packed_data[0], BRIGHTSIDE_VOLTAGE_SENSORS_DATA_LENGTH);
    printf("data1: %f\n", unpacked_data.volt_sensor1);
    printf("data2: %f\n", unpacked_data.volt_sensor2);
    printf("brightside_voltage_sensors_data_unpack: %d\n", status);


    return 0;
}