/*
 * cpu_load.c
 *
 *  Created on: Sep 19, 2024
 *      Author: diego
 */

#include "cpu_load.h"
#include "circular_buffer.h"

#define FLOAT_TO_PERCENTAGE 100.0f

static uint16_t idleRunTime = 0;
static uint16_t idle_switched_in = 0;
static uint16_t window_start_time = 0;
static uint16_t total_run_time = 0;

void taskSwitchedIn(){
	TaskHandle_t idleTaskHandle = xTaskGetIdleTaskHandle();
	if(xTaskGetCurrentTaskHandle() == idleTaskHandle){
		idle_switched_in = __HAL_TIM_GET_COUNTER(&htim10);
	}
}


void taskSwitchedOut(){
	TaskHandle_t idleTaskHandle = xTaskGetIdleTaskHandle();
	if(xTaskGetCurrentTaskHandle() == idleTaskHandle){
		idleRunTime += __HAL_TIM_GET_COUNTER(&htim10) - idle_switched_in;
	}
}

void CPULoadConfig(uint8_t window_size_ms){

}



void resetWindow(){
	idleRunTime = 0;
	window_start_time = __HAL_TIM_GET_COUNTER(&htim10);
}

float calculateCPULoad(uint16_t window_size_ms){

	total_run_time = __HAL_TIM_GET_COUNTER(&htim10) - window_start_time;

	if(total_run_time >= window_size_ms){
		float cpu_load = (1.0f - ((float)idleRunTime / total_run_time)) * FLOAT_TO_PERCENTAGE;
		resetWindow();

		return cpu_load;
	}
	else{
		return -1;
	}
}
