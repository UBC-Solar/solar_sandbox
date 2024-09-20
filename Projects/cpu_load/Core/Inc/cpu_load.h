/*
 * cpu_load.h
 *
 *  Created on: Sep 19, 2024
 *      Author: diego
 */

#ifndef INC_CPU_LOAD_H_
#define INC_CPU_LOAD_H_

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx.h"

extern TIM_HandleTypeDef htim10;

void taskSwitchedIn(void);
void taskSwitchedOut(void);
float calculateCPULoad(void);


#endif /* INC_CPU_LOAD_H_ */
