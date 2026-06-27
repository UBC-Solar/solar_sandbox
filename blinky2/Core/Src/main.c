/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2c_receive.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
volatile HAL_StatusTypeDef i2c1_ready = HAL_ERROR;
volatile HAL_StatusTypeDef i2c1_read_status = HAL_ERROR;
volatile uint32_t i2c1_error = HAL_I2C_ERROR_NONE;
uint8_t receive_buffer[GPS_MESSAGE_LEN + 1];
volatile uint8_t gps_parse_ok = 0;
volatile float gps_latitude = 0.0f;
volatile float gps_longitude = 0.0f;
volatile uint16_t gps_dollar_count = 0;
volatile uint8_t gps_gga_found = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static float nmea_coord_to_decimal(float nmea_coord)
{
  int degrees = (int)(nmea_coord / 100.0f);
  float minutes = nmea_coord - ((float)degrees * 100.0f);

  return (float)degrees + (minutes / 60.0f);
}

static char *find_gga_sentence(uint8_t *buffer, size_t len)
{
  const char gngga[] = "$GNGGA";
  const char gpgga[] = "$GPGGA";
  size_t sentence_id_len = sizeof(gngga) - 1;

  if (len < sentence_id_len) return NULL;

  for (size_t i = 0; i <= len - sentence_id_len; i++)
  {
    if (memcmp(&buffer[i], gngga, sentence_id_len) == 0 ||
        memcmp(&buffer[i], gpgga, sentence_id_len) == 0)
    {
      return (char *)&buffer[i];
    }
  }

  return NULL;
}

static uint8_t parse_gga_lat_lon(uint8_t *buffer, size_t len, float *latitude, float *longitude)
{
  char *gga = find_gga_sentence(buffer, len);
  if (!gga) return 0;

  char sentence[120];
  char talker[8];
  char utc_time[16];
  char lat_raw_str[16];
  char lon_raw_str[16];
  char lat_side = '\0';
  char lon_side = '\0';

  size_t max_sentence_len = len - (size_t)(gga - (char *)buffer);
  size_t sentence_len = 0;

  while (sentence_len < max_sentence_len &&
         sentence_len < sizeof(sentence) - 1 &&
         gga[sentence_len] != '\r' &&
         gga[sentence_len] != '\n' &&
         gga[sentence_len] != '\0')
  {
    sentence_len++;
  }

  memcpy(sentence, gga, sentence_len);
  sentence[sentence_len] = '\0';

  int matched = sscanf(sentence, "$%7[^,],%15[^,],%15[^,],%c,%15[^,],%c",
                       talker, utc_time, lat_raw_str, &lat_side, lon_raw_str, &lon_side);

  if (matched != 6) return 0;

  float lat_raw = strtof(lat_raw_str, NULL);
  float lon_raw = strtof(lon_raw_str, NULL);

  if (lat_raw == 0.0f || lon_raw == 0.0f) return 0;

  float lat = nmea_coord_to_decimal(lat_raw);
  float lon = nmea_coord_to_decimal(lon_raw);

  if (lat_side == 'S') lat = -lat;
  if (lon_side == 'W') lon = -lon;

  *latitude = lat;
  *longitude = lon;

  return 1;
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  HAL_Delay(5000);
  i2c1_ready = gps_check_ready();
  if (i2c1_ready == HAL_OK)
  {
    gps_config_status = gps_configure_nmea_250ms();
    HAL_Delay(500);
  }
  i2c1_error = HAL_I2C_GetError(&hi2c1);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, (i2c1_ready == HAL_OK) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    memset(receive_buffer, 0, sizeof(receive_buffer));

    i2c1_read_status = read_i2c_gps_module(receive_buffer);
    receive_buffer[GPS_MESSAGE_LEN] = '\0';
    i2c1_error = HAL_I2C_GetError(&hi2c1);
    gps_dollar_count = 0;
    for (size_t i = 0; i < GPS_MESSAGE_LEN; i++)
    {
      if (receive_buffer[i] == '$') gps_dollar_count++;
    }
    gps_gga_found = (find_gga_sentence(receive_buffer, GPS_MESSAGE_LEN) != NULL);

    if (i2c1_read_status == HAL_OK) HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    else HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    if (i2c1_read_status == HAL_OK)
    {
      gps_parse_ok = parse_gga_lat_lon(receive_buffer, GPS_MESSAGE_LEN,
                                       (float *)&gps_latitude,
                                       (float *)&gps_longitude);
    }
    else
    {
      gps_parse_ok = 0;
    }

    HAL_Delay(250);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
#ifdef USE_FULL_ASSERT
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
