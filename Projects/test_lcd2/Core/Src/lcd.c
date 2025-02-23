// /**
//  * References the library: https://github.com/mberntsen/STM32-Libraries
//  */

// #include "spi.h"
// #include "lcd.h"
// #include "main.h"
// #include <stdio.h>
// #include "st7565.h"
// #include "graphics.h"
// #include "font_tahoma.h"

// #define SPEED_FONT    (Verdana32)
// #define SPEED_X     36
// #define SPEED_Y     29
// #define SPEED_SPACING   2
// #define BOTTOM_RIGHT_X      127
// #define BOTTOM_RIGHT_Y      63


// /** START FIELD DISPLAYING FUNCTION DEFINITIONS */

// /**
//  * @brief Internal function in the init to print the fields 
//  */
// void print_fields()
// {
//     LCD_print(FIELD_START, FIELD_0_LINE, FIELD_0);     // Field 0
//     LCD_print(FIELD_START, FIELD_1_LINE, FIELD_1);     // Field 1
// }

// /** FIELD 0 */
// void LCD_display_data_field_0(uint32_t speed)
// {
//     // convert speed to string.
//     char speed_str[12];  // Buffer to hold the converted string (enough for 10 digits + null terminator)
//     sprintf(speed_str, "%02lu", (unsigned long)speed);  // Convert uint32_t to string

//     LCD_print(FIELD_0_DATA_POS, FIELD_0_LINE, speed_str);     // Field 1
// }

// static bounding_box_t old_bb_speed = {0};
// static bounding_box_t old_bb_speed_units = {0};
// #define LCD_SPEED_UNITS_MPH     0
// #define LCD_SPEED_UNITS_KPH     1
// #define SPEED_UNITS_FONT      (Verdana8)
// #define SPEED_UNITS_SPACING     1

// void LCD_display_speed(uint32_t speed, int units)
// {
//     char speed_str[12];  // Buffer to hold the converted string (enough for 10 digits + null terminator)
//     glcd_clear_bounding_box(SPEED_X - SPEED_SPACING, SPEED_Y, old_bb_speed.x2, BOTTOM_RIGHT_Y);
//     glcd_clear_bounding_box(old_bb_speed.x2, SPEED_Y, old_bb_speed_units.x2 + SPEED_UNITS_SPACING, SPEED_Y + 11);
//     if (speed < 10)         // Single digit
//     {
//         sprintf(speed_str, "%01lu", (unsigned long)speed);  
//         old_bb_speed = draw_text(speed_str, SPEED_X + 14, SPEED_Y, SPEED_FONT, SPEED_SPACING);      // Redraw it
//     }
//     else
//     {
//         sprintf(speed_str, "%02lu", (unsigned long)speed);  
//         old_bb_speed = draw_text(speed_str, SPEED_X, SPEED_Y, SPEED_FONT, SPEED_SPACING);      // Redraw it
//     }
    
//     switch (units)  // Draw in the units
//     {
//         case LCD_SPEED_UNITS_KPH:
//             old_bb_speed_units = draw_text("kph", old_bb_speed.x2 + SPEED_SPACING, SPEED_Y, SPEED_UNITS_FONT, SPEED_UNITS_SPACING);  
//             break;
            
//         case LCD_SPEED_UNITS_MPH:
//             old_bb_speed_units = draw_text("mph", old_bb_speed.x2 + SPEED_SPACING, SPEED_Y, SPEED_UNITS_FONT, SPEED_UNITS_SPACING);  
//             break;
            
//         default:
//             old_bb_speed_units = draw_text("xxx", old_bb_speed.x2 + SPEED_SPACING, SPEED_Y, SPEED_UNITS_FONT, SPEED_UNITS_SPACING);  
//             break;
//     }

//     glcd_refresh();
// }

// /* FROM CAN ID TABLE */
// #define STATE_IDX       0
// #define FORWARD_STATE     0x01    
// #define FORWARD_SYMBOL    'D'    
// #define PARK_STATE      0x03        
// #define PARK_SYMBOL    'P'    
// #define REVERSE_STATE     0x04        
// #define REVERSE_SYMBOL    'R'    
// #define ERROR_SYMBOL    'X'    
// #define STATE_X         1
// #define STATE_Y         40
// #define STATE_FONT      (Verdana22)

// static bounding_box_t old_bb_drive_state = {0};
// void LCD_display_drive_state(int state)
// {
//     char state_str[2] = {ERROR_SYMBOL, '\0'};       // Assume error
//     glcd_clear_bounding_box(STATE_X, STATE_Y, old_bb_drive_state.x2, BOTTOM_RIGHT_Y);
//     switch (state)
//     {
//         case FORWARD_STATE:
//         state_str[STATE_IDX] = FORWARD_SYMBOL;
//         break;
        
//         case PARK_STATE:
//         state_str[STATE_IDX] = PARK_SYMBOL; 
//         break;
        
//         case REVERSE_STATE:
//         state_str[STATE_IDX] = REVERSE_SYMBOL;
//         break;
        
//         default:
//         break;
//     }
    
//     old_bb_drive_state = draw_text(state_str, STATE_X, STATE_Y, STATE_FONT, STATE_FONT);      // Redraw it
//     glcd_refresh();
// }

// #define SOC_FONT    (Verdana16)
// #define SOC_X     95
// #define SOC_Y     46
// #define SOC_SPACING   1
// #define SOC_UNITS_FONT      (Verdana8)
// #define SOC_UNITS      '%'
// static bounding_box_t old_bb_soc = {0};
// void LCD_display_SOC(uint32_t soc)
// {
//     char soc_str[12];  // Buffer to hold the converted string (enough for 10 digits + null terminator)
//     bounding_box_t bb;
//     glcd_clear_bounding_box(SOC_X - SOC_SPACING, SOC_Y, BOTTOM_RIGHT_X, BOTTOM_RIGHT_Y);
//     if (soc < 10)         // Single digit
//     {
//         sprintf(soc_str, "%01lu", (unsigned long)soc);  
//         bb = draw_text(soc_str, SOC_X + 10, SOC_Y, SOC_FONT, SOC_SPACING);      // Redraw it
//     }
//     else
//     {
//         sprintf(soc_str, "%02lu", (unsigned long)soc);  
//         bb = draw_text(soc_str, SOC_X, SOC_Y, SOC_FONT, SOC_SPACING);      // Redraw it
//     }
    
//     old_bb_soc = draw_char(SOC_UNITS, bb.x2 + 2, SOC_Y, SOC_UNITS_FONT);
//     glcd_refresh();
// }


// #define MAX_POSITIVE_POWER 5400.0f
// #define MAX_NEGATIVE_POWER 3000.0f   // use the absolute value for negative power
// #define BAR_LEFT 1
// #define BAR_TOP 1
// #define BAR_BOTTOM 20
// #define BAR_RIGHT BOTTOM_RIGHT_X
// #define CENTER_X 43

// void LCD_display_power_bar(float pack_current, float pack_voltage)
// {
//     // Compute battery power (which may be positive or negative)
//     float power = pack_current * pack_voltage;

//     // Clear the entire drawing area for the bar,
//     // including extra space for the center line below the bar.
//     glcd_clear_bounding_box(BAR_LEFT, BAR_TOP, BAR_RIGHT, BAR_BOTTOM + 3);

//     // Draw the outline of the bar rectangle.
//     draw_rectangle(BAR_LEFT, BAR_TOP, BAR_RIGHT, BAR_BOTTOM, 1);

//     // Depending on whether the power is positive or negative,
//     // calculate the fill distance relative to the center line.
//     if (power > 0) {
//         // For positive power, fill from CENTER_X + 1 to the right.
//         float ratio = power / MAX_POSITIVE_POWER;
//         if (ratio > 1.0f)
//             ratio = 1.0f;
//         int total_pixels_right = BAR_RIGHT - CENTER_X;
//         int fill_pixels = (int)(ratio * total_pixels_right);
//         // Fill the interior (leaving the top and bottom outline intact).
//         for (int y = BAR_TOP + 1; y < BAR_BOTTOM; y++) {
//             for (int x = CENTER_X + 1; x <= CENTER_X + fill_pixels; x++) {
//                 glcd_pixel(x, y, 1);
//             }
//         }
//     } else if (power < 0) {
//         // For negative power, fill from CENTER_X - 1 to the left.
//         float ratio = (-power) / MAX_NEGATIVE_POWER;
//         if (ratio > 1.0f)
//             ratio = 1.0f;
//         int total_pixels_left = CENTER_X - BAR_LEFT;
//         int fill_pixels = (int)(ratio * total_pixels_left);
//         for (int y = BAR_TOP + 1; y < BAR_BOTTOM; y++) {
//             for (int x = CENTER_X - 1; x >= CENTER_X - fill_pixels; x--) {
//                 glcd_pixel(x, y, 1);
//             }
//         }
//     }

//     // Draw the center line at x = CENTER_X.
//     // Make it extend from the top of the rectangle down to 3 pixels below the rectangle.
//     for (int y = BAR_TOP; y <= BAR_BOTTOM + 3; y++) {
//         glcd_pixel(CENTER_X, y, 1);
//     }

//     // Refresh the display to show changes.
//     glcd_refresh();
// }



// /** FIELD 0 */
// void LCD_display_data_field_1(float throttle_percent)
// {
//     char throttle_str[8];  // Buffer to hold the formatted string (e.g., "100.0%" is 6 chars + null terminator)
//     sprintf(throttle_str, "%.1f%%", throttle_percent);  // Format as a percentage with 1 decimal place

//     LCD_print(FIELD_1_DATA_POS, FIELD_1_LINE, throttle_str);  // Display throttle percentage
// }

// /** END FIELD DISPLAYING FUNCTION DEFINITIONS */

// /** BASE LOGIC FOR LCD USAGE */

// static SPI_HandleTypeDef* sg_spi_handle;

// /**
//  * @brief Sends specificed command to LCD screen via SPI
//  * 
//  * @param cmd Command to send. See Page 8 on https://newhavendisplay.com/content/specs/NHD-C12864A1Z-FSW-FBW-HTT.pdf
//  */
// void LCD_write_command(uint8_t cmd) {
//     // HAL_GPIO_WritePin(CS, CS_Pin, GPIO_PIN_RESET); // CS = 0 to select
//     HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_RESET); // A0 = 0 for command

//     // TODO: Send `cmd` over SPI (not implemented yet)
//     uint8_t cmd_arr[1] = {cmd};
//     HAL_SPI_Transmit(sg_spi_handle, &cmd_arr[0], 1, 10);

//     // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // CS = 0 to select
// }

// /**
//  * @brief Sends data to LCD screen via SPI so that the specified pixels turn black (or white)
//  * 
//  * @param cmd Command to send. See this update on how writing data works: 
//  *            https://ubcsolar26.monday.com/boards/7524367653/pulses/7915667617/posts/3834350358
//  */
// void LCD_write_data(uint8_t data) {
//     // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // CS = 0 to select
//     HAL_GPIO_WritePin(A0_GPIO_Port, A0_Pin, GPIO_PIN_SET); // A0 = A0 = 1 for data

//     uint8_t data_arr[1] = {data};
//     HAL_SPI_Transmit(sg_spi_handle, &data_arr[0], 1, 10);
//     // SPI_Write(data); // Send command manually

//     // HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // CS = 0 to select
// }

// /**
//  * @brief Performs an LCD init based on 
//  *        https://github.com/NewhavenDisplay/NHD-C12864A1Z_Example/blob/main/NHD-C12864A1Z/NHD-C12864A1Z.ino#L197
//  * 
//  *        Also prints all the field names to get ready to display their data.
//  */
// void LCD_init(SPI_HandleTypeDef* hspi)
// {
//     sg_spi_handle = hspi;
//     ST7565_begin();
// }

// /**
//  * @brief Sends data to LCD screen via SPI so that the specified pixels turn black (or white)
//  * 
//  * @param x The position along the line to start printing. Typically a multiple of CHAR_WIDTH
//  * @param line The line to do the printing on
//  * @param c Pointer to character array that you want to print
//  */
// void LCD_print(uint8_t x, uint8_t line, char *c) {
//     ST7565_drawstring(x, line, c);
//     ST7565_display();
// }

// /**
//  * @brief Clears the LCD display
//  */
// void LCD_clear()
// {
//     ST7565_clear_display();
// }

