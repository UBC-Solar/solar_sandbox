/**
 * References the library: https://github.com/mberntsen/STM32-Libraries
 */

#include "lcd.h"
#include "main.h"
#include "spi.h"
#include <stdio.h>

void LCD_write_command(uint8_t cmd) {
    // HAL_GPIO_WritePin(CS, CS_Pin, GPIO_PIN_RESET); // CS = 0 to select
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_RESET); // A0 = 0 for command

    // TODO: Send `cmd` over SPI (not implemented yet)
    uint8_t cmd_arr[1] = {cmd};
    HAL_SPI_Transmit(&hspi2, &cmd_arr[0], 1, 10);

    // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // CS = 0 to select
}

void LCD_write_data(uint8_t data) {
    // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // CS = 0 to select
    HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET); // A0 = A0 = 1 for data

    uint8_t data_arr[1] = {data};
    HAL_SPI_Transmit(&hspi2, &data_arr[0], 1, 10);
    // SPI_Write(data); // Send command manually

    // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // CS = 0 to select
}

void LCD_display_data_field1(uint32_t speed)
{
    // convert speed to string.
    char speed_str[12];  // Buffer to hold the converted string (enough for 10 digits + null terminator)
    sprintf(speed_str, "%lu", (unsigned long)speed);  // Convert uint32_t to string

    LCD_print(FIELD_1_DATA_POS, FIELD_1_LINE, speed_str);     // Field 1
}


void LCD_display_data_field2(float throttle_percent)
{
    char throttle_str[8];  // Buffer to hold the formatted string (e.g., "100.0%" is 6 chars + null terminator)
    sprintf(throttle_str, "%.1f%%", throttle_percent);  // Format as a percentage with 1 decimal place

    LCD_print(FIELD_2_DATA_POS, FIELD_2_LINE, throttle_str);  // Display throttle percentage
}


void print_fields()
{
    LCD_print(FIELD_START, FIELD_1_LINE, FIELD_1);     // Field 1
    LCD_print(FIELD_START, FIELD_2_LINE, FIELD_2);     // Field 2
}

void LCD_init()
{
    ST7565_begin();
    print_fields();
}


void LCD_print(uint8_t x, uint8_t line, char *c) {
    ST7565_drawstring(x, line, c);
    ST7565_display();
}

void LCD_clear()
{
    ST7565_clear_display();
}
