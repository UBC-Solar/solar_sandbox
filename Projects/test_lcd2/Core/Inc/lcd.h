#ifndef __LCD_H__
#define __LCD_H__

#include "ST7565.h"
#include <stdint.h>


/** DISPLAY SETTINGS */
#define CHAR_WIDTH          6
#define FIELD_START         0


/** START DISPLAY FIELDS */

/** FIELD 1 */
#define FIELD_1       "Speed: "
#define FIELD_1_DATA_POS   (7 * CHAR_WIDTH)
#define FIELD_1_LINE  0

/** FIELD 2 */
#define FIELD_2       "Other Data: "
#define FIELD_2_DATA_POS   (12 * CHAR_WIDTH)
#define FIELD_2_LINE  1

/** FIELD 3 */

/** END DISPLAY FIELDS */

void LCD_init();
void LCD_display_data_field1(uint32_t speed);
void LCD_display_data_field2(float throttle_percent);
void LCD_print(uint8_t x, uint8_t line, char *c);
void LCD_clear();
void LCD_write_command(uint8_t cmd);
void LCD_write_data(uint8_t data);


#endif /*__ LCD_H__ */
