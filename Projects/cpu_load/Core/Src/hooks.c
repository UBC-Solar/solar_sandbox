/*
 * hooks.c
 *
 *  Created on: Aug 14, 2024
 *      Author: Diego
 */

#include <stdbool.h>
#include "hooks.h"

static uint16_t start_idle_time = 0;
static uint16_t idle_time = 0;
static bool first_run = true;

void vApplicationIdleHook(void) {

	uint16_t current_time = __HAL_TIM_GET_COUNTER(&htim10);

	if(first_run){
		start_idle_time = __HAL_TIM_GET_COUNTER(&htim10);
		first_run = false;
	}
	else{
		idle_time += (current_time - start_idle_time);
		start_idle_time = current_time;
	}
}


float calculateCPULoad(void){
	uint16_t total_time = __HAL_TIM_GET_COUNTER(&htim10);
	float cpu_load = 1.0f - ((float)idle_time / (float)total_time);

	return cpu_load * 100.0f;
}
