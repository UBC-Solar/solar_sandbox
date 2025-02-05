/**
 * References the library: https://github.com/mberntsen/STM32-Libraries
 */

#include "lcd.h"
#include "main.h"
#include "spi.h"

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

void LCD_init()
{
    ST7565_begin();
}

void LCD_print(uint8_t x, uint8_t line, char *c) {
    ST7565_drawstring(x, line, c);
    ST7565_display();
}

void LCD_clear()
{
    ST7565_clear_display();
}
