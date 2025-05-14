#include "soc.h"
#include <stdio.h>

#define HPPC_DATA_CSV   "fsgp_data.csv"
#define SOC_OUTPUT_CSV   "soc.csv"

int main()
{
    FILE* f_hppc_data = fopen(HPPC_DATA_CSV, "r");
    FILE* f_soc = fopen(SOC_OUTPUT_CSV, "w");

    // Read line by line. CURRENT IS FIRST. then voltage
    float current;
    float voltage;
    float soc;
    while(fscanf(f_hppc_data, "%f %f", &current, &voltage) == 2)
    {
        // Send each of these into SoC algorithm. Output SoC into output csv
        SOC_predict_then_update(current, voltage, SOC_TIME_STEP);
        soc = SOC_get_soc();

        // Write to SoC output file
        fprintf(f_soc, "%f\n", soc);
    }


    // void SOC_predict_then_update(float g_total_pack_voltage_soc, float g_pack_current_soc, float time_step);


    return 0;
}