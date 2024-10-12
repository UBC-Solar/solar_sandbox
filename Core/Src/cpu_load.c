/*
 * cpu_load.c
 *
 *  Created on: Sep 19, 2024
 *      Author: diego
 */

#include "cpu_load.h"
#include "circular_buffer.h"

#define FLOAT_TO_PERCENTAGE 100.0f
#define MAX_CPU_LOAD_ENTRIES 20

static uint16_t idleRunTime = 0;
static uint16_t idle_switched_in = 0;
static uint16_t window_start_time = 0;
static uint16_t total_run_time = 0;

static float cpu_load_buffer[MAX_CPU_LOAD_ENTRIES];

circ_buf_t cpuLoadBuffer = {
    .pBuffer = cpu_load_buffer,
    .head = 0,
    .tail = 0,
    .num_entries = 0,
};

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

void reset(){
	idleRunTime = 0;
	window_start_time = __HAL_TIM_GET_COUNTER(&htim10);
}

float calculateCPULoad(uint8_t window_size){

	total_run_time = __HAL_TIM_GET_COUNTER(&htim10) - window_start_time;

	if(total_run_time >= window_size_ms){
		float cpu_load = (1.0f - ((float)idleRunTime / total_run_time)) * FLOAT_TO_PERCENTAGE;

		circ_buf_enqueue(&cpuLoadBuffer, cpu_load, window_size);

		reset();

		return cpu_load;
	}
	else{
		return -1;
	}
}

float getAverageCPULoad() {
    if (circ_buf_empty(&cpuLoadBuffer)) {
        return -1;  // Return an indicator for no data in buffer
    }

    float sum = 0.0f;
    for (int i = 0; i < cpuLoadBuffer.num_entries; i++) {
        sum += cpuLoadBuffer.pBuffer[(cpuLoadBuffer.tail + i) % MAX_CPU_LOAD_ENTRIES];
    }

    return sum / cpuLoadBuffer.num_entries;
}












