#ifndef __LCD_H__
#define __LCD_H__

#include "ST7565.h"

void LCD_init();
void LCD_print(uint8_t x, uint8_t line, char *c);
void LCD_clear();
void LCD_write_command(uint8_t cmd);
void LCD_write_data(uint8_t data);


#endif /*__ LCD_H__ */
