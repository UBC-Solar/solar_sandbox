/*
 * hooks.h
 *
 *  Created on: Aug 13, 2024
 *      Author: Diego
 */

#ifndef INC_HOOKS_H_
#define INC_HOOKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx.h"

extern TIM_HandleTypeDef htim10;

void vApplicationIdleHook(void);
float calculateCPULoad(void);

#endif /* INC_HOOKS_H_ */

