#include "soc.h"
#include <stdio.h>

#define HPPC_DATA_CSV   "fsgp_data.csv"
#define SOC_OUTPUT_CSV   "soc.csv"
#define VOLTAGE_OUTPUT_CSV   "last_voltage.csv"

#define CURRENT_CSV "current.csv"
#define VOLTAGE_CSV "voltage.csv"

int main()
{
    FILE* f_current = fopen(CURRENT_CSV, "r");
    FILE* f_voltage = fopen(VOLTAGE_CSV, "r");
    FILE* f_voltage_out = fopen(VOLTAGE_OUTPUT_CSV, "w");
    FILE* f_soc = fopen(SOC_OUTPUT_CSV, "w");

    // Read line by line. CURRENT IS FIRST. then voltage
    float current;
    float voltage;
    float soc;
    float last_voltage;
    fscanf(f_current, "%*[^,],%f", &current);   // Get rid of the time value
    fscanf(f_voltage, "%*[^,],%f", &voltage);
    while(fscanf(f_current, "%*[^,],%f", &current) == 1)
    {
        fscanf(f_voltage, "%*[^,],%f", &voltage);

        // Send each of these into SoC algorithm. Output SoC into output csv
        SOC_predict_then_update(voltage, current, SOC_TIME_STEP);
        soc = SOC_get_soc();
        last_voltage = SOC_get_voltage();


        // Write to SoC output file
        fprintf(f_soc, "%.20f\n", soc);
        fprintf(f_voltage_out, "%.20f\n", last_voltage);
    }


    // void SOC_predict_then_update(float g_total_pack_voltage_soc, float g_pack_current_soc, float time_step);


    return 0;
}