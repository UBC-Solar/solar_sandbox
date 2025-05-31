/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */
uint16_t adc_buffer[NUM_SAMPLES + NUM_DELIMITER_UINT16];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool uart_tx_flag;


uint64_t Read_ADC_Value(void)
{
    uint64_t value = 0;

    // Poll for conversion complete (timeout = 10ms)
    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        // Timeout or error
        return 0;
    }

    value = HAL_ADC_GetValue(&hadc1);
    // Check if conversion is finished
    // if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
    // {
    //     // Get the converted value
    //     value = HAL_ADC_GetValue(&hadc1);
    // }


    return value;
}
uint64_t Read_ADC_Value2(void)
{
    uint64_t value = 0;

    // Poll for conversion complete (timeout = 10ms)
    if (HAL_ADC_PollForConversion(&hadc2, 100) != HAL_OK)
    {
        // Timeout or error
        return 0;
    }

    value = HAL_ADC_GetValue(&hadc2);
    // Check if conversion is finished
    // if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC) == HAL_ADC_STATE_REG_EOC)
    // {
    //     // Get the converted value
    //     value = HAL_ADC_GetValue(&hadc1);
    // }


    return value;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_UART5_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  adc_buffer[NUM_SAMPLES + 0] = (uint16_t)(0xffff);
  adc_buffer[NUM_SAMPLES + 1] = (uint16_t)(0xff0a);

    // Start conversion for basic
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
    }

    // Start conversion for basic
    if (HAL_ADC_Start(&hadc2) != HAL_OK)
    {
    }


// HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, NUM_SAMPLES);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    
        // HAL_GPIO_TogglePin(DEBUG1_GPIO_Port, DEBUG1_Pin);       // Debug 1 blinking means Uart is done
        // HAL_Delay(100);
    
        uint64_t adcVal = Read_ADC_Value();
        uint64_t adcVal2 = Read_ADC_Value2();
        uint64_t x = (adcVal << 32) | adcVal2;
        HAL_UART_Transmit(&huart5, (uint8_t *)&x, sizeof(x), 5);
        // HAL_UART_Transmit(&huart5, (uint8_t *)&adcVal2, sizeof(adcVal2), 5);
        HAL_Delay(10);

        // char c[] = "Hello World";
        // HAL_UART_Transmit(&huart5, (uint8_t *)c, 12, 50);
        // HAL_GPIO_TogglePin(DEBUG1_GPIO_Port, DEBUG1_Pin);       // Debug 1 blinking means Uart is done
        // HAL_Delay(100);
    
        // if (uart_tx_flag)
        // {
        //     uart_tx_flag = false;
        //     HAL_UART_Transmit(&huart5, (uint8_t *)&adc_buffer[0], sizeof(adc_buffer), 1000);
        //     HAL_GPIO_WritePin(DEBUG1_GPIO_Port, DEBUG1_Pin, 0);       // Debug 1 blinking means Uart is done
        // }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
