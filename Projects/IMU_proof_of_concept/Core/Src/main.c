/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
DMA_HandleTypeDef hdma_i2c1_rx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define IMU_ADDRESS (0x6B << 1)

#define OUTX_L_G 0x22 //Angular rate x-axis register address lower 8 bits
#define OUTX_H_G 0x23 //Angular rate x-axis register address upper 8 bits
#define OUTY_L_G 0x24 //Angular rate y-axis register address
#define OUTY_H_G 0x25 //Angular rate y-axis register address
#define OUTZ_L_G 0x26 //Angular rate z-axis register address
#define OUTZ_H_G 0x27 //Angular rate z-axis register address

#define OUTX_L_A 0x28 //Linear Acceleration x-axis register address lower 8 bits
#define OUTX_H_A 0x29 //Linear Acceleration x-axis register address upper 8 bits
#define OUTY_L_A 0x2A //Linear Acceleration y-axis register address
#define OUTY_H_A 0x2B //Linear Acceleration y-axis register address
#define OUTZ_L_A 0x2C //Linear Acceleration z-axis register address
#define OUTZ_H_A 0x2D //Linear Acceleration z-axis register address

#define CTRL_1 0x10 //Address of CTRL1 register (Sets ODR for linear accel)
#define CTRL_2 0x11 //Address of CTRL1 register (Sets ODR for angular rate)

#define CTRL_8 0x17 //Address of CTRL8 register (Sets full scale range for linear accel)
#define CTRL_6 0x15 //Address of CTRL6 register (Sets full scale range for angular rate)





/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	int8_t bufx[2];
	int8_t bufy[2];
	int8_t bufz[2];
	int8_t debug_buf[200];
	HAL_StatusTypeDef retx;
	HAL_StatusTypeDef rety;
	HAL_StatusTypeDef retz;

//	typedef struct  {
//	  float x;
//	  float y;
//	  float z;
//	};


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	/* USER CODE END WHILE */

	/* USER CODE BEGIN 3 */
	// Read OUTX_L_A and OUTX_H_A
	  	// Write to CTRL1 register
		bufx[0] = 0x74; // Set ODR to 30 Hz, normal mode
		retx = HAL_I2C_Mem_Write(&hi2c1, IMU_ADDRESS, CTRL_1, 1, bufx, 1, 1000);
		bufy[0] = 0x02; // Set the range to +-8g (might have to change later - check if sufficient for cornering?)
		rety = HAL_I2C_Mem_Write(&hi2c1, IMU_ADDRESS, CTRL_8, 1, bufy, 1, 1000);

		if (retx != HAL_OK) {
			sprintf((char*)debug_buf, "Error writing CTRL1: %d\r\n", retx);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}
		if (rety != HAL_OK) {
			sprintf((char*)debug_buf, "Error writing CTRL8: %d\r\n", rety);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}

		retx = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTX_L_A, 1, bufx, 2, 1000);
		rety = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTY_L_A, 1, bufy, 2, 1000);
		retz = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTZ_L_A, 1, bufz, 2, 1000);

		if (retx != HAL_OK || rety != HAL_OK || retz != HAL_OK) {
			sprintf((char*)debug_buf, "Error reading accel: %d\r\n", retx);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}


		else {
			int16_t Accel_X_RAW = (int16_t)(bufx[1] << 8 | bufx[0]);
			int16_t Accel_Y_RAW = (int16_t)(bufy[1] << 8 | bufy[0]);
			int16_t Accel_Z_RAW = (int16_t)(bufz[1] << 8 | bufz[0]);

			float accel_x = Accel_X_RAW * 0.488; // Convert to mg (Multiply by Datasheet value of 0.488)
			float accel_y = Accel_Y_RAW * 0.488; // Convert to mg
			float accel_z = Accel_Z_RAW * 0.488; // Convert to mg

			sprintf((char*)debug_buf, "Accel X: %.2f mg\t Accel Y: %.2f mg\t Accel Z: %.2f mg\r\n", accel_x, accel_y, accel_z);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}

		// Write to CTRL2 register
		bufx[0] = 0x54; // Set ODR to 30HZ, low-power mode;
		retx = HAL_I2C_Mem_Write(&hi2c1, IMU_ADDRESS, CTRL_2, 1, bufx, 1, 1000);
		bufy[0] = 0x01; // Set the scale to +-500 degrees per sec (dps) (might have to change later - check if sufficient for cornering?)
		rety = HAL_I2C_Mem_Write(&hi2c1, IMU_ADDRESS, CTRL_6, 1, bufy, 1, 1000);

		if (retx != HAL_OK) {
			sprintf((char*)debug_buf, "Error writing CTRL2: %d\r\n", retx);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}

		if (rety != HAL_OK) {
			sprintf((char*)debug_buf, "Error writing CTRL6: %d\r\n", rety);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}

		retx = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTX_L_G, 1, bufx, 2, 1000);
		rety = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTY_L_G, 1, bufy, 2, 1000);
		retz = HAL_I2C_Mem_Read(&hi2c1, IMU_ADDRESS, OUTZ_L_G, 1, bufz, 2, 1000);

		if (retx != HAL_OK || rety != HAL_OK || retz != HAL_OK) {
			sprintf((char*)debug_buf, "Error reading gyro: %d\r\n", retx);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
		}


		else {
			int16_t Gyro_X_RAW = (int16_t)(bufx[1] << 8 | bufx[0]);
			int16_t Gyro_Y_RAW = (int16_t)(bufy[1] << 8 | bufy[0]);
			int16_t Gyro_Z_RAW = (int16_t)(bufz[1] << 8 | bufz[0]);

			float gyro_x = Gyro_X_RAW * 8.75; // Convert to milli degrees per sec (mdps) (Multiply by Datasheet value of 8.75)
			float gyro_y = Gyro_Y_RAW * 8.75; // Convert to mdps
			float gyro_z = Gyro_Z_RAW * 8.75; // Convert to mdps

			sprintf((char*)debug_buf, "Gyro X: %.2f mdps\t Gyro Y: %.2f mdps\t Gyro Z: %.2f mdps\r\n", gyro_x, gyro_y, gyro_z);
			HAL_UART_Transmit(&huart2, debug_buf, strlen((char*)debug_buf), 1000);
			osDelay(2000);
		}

	}  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
