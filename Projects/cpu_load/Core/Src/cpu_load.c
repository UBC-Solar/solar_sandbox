/*
 * cpu_load.c
 *
 *  Created on: Sep 19, 2024
 *      Author: diego
 */

// static uint32_t idleRunTime =0;

#include "cpu_load.h"

#define FLOAT_TO_PERCENTAGE 100.0f



void taskSwitchedIn(){
	TaskHandle_t idleTaskHandle = xTaskGetIdleTaskHandle();
	if(xTaskGetCurrentTaskHandle() == idleTaskHandle){
		HAL_TIM_Base_Start(&htim10);
	}
}


void taskSwitchedOut(){
	TaskHandle_t idleTaskHandle = xTaskGetIdleTaskHandle();
	if(xTaskGetCurrentTaskHandle() == idleTaskHandle){
		HAL_TIM_Base_Stop(&htim10);
	}
}


float calculateCPULoad(){

	uint32_t totalRunTime = HAL_GetTick();
	uint32_t idleRunTime = __HAL_TIM_GET_COUNTER(&htim10);

	float cpu_load = (1.0f - ((float)idleRunTime / (float)totalRunTime)) * FLOAT_TO_PERCENTAGE;

	return cpu_load;
}
