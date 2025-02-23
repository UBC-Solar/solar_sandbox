#include "main.h"
#include "st7565-config.h"
#include "st7565.h"
#include "lcd.h"


/** Global buffer to hold the current screen contents. */
// This has to be kept here because the width & height are set in
// st7565-config.h
unsigned char glcd_buffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];
static unsigned char glcd_flipped = 0;


#ifdef ST7565_DIRTY_PAGES
unsigned char glcd_dirty_pages;
#endif

void glcd_pixel(unsigned char x, unsigned char y, unsigned char colour) {

    if (x > SCREEN_WIDTH || y > SCREEN_HEIGHT) return;

    // Real screen coordinates are 0-63, not 1-64.
    x -= 1;
    y -= 1;

    unsigned short array_pos = x + ((y / 8) * 128);

#ifdef ST7565_DIRTY_PAGES
#warning ** ST7565_DIRTY_PAGES enabled, only changed pages will be written to the GLCD **
    glcd_dirty_pages |= 1 << (array_pos / 128);
#endif

    if (colour) {
        glcd_buffer[array_pos] |= 1 << (y % 8);
    } else {
        glcd_buffer[array_pos] &= 0xFF ^ 1 << (y % 8);
    }
}

void glcd_blank() {
    // Reset the internal buffer
    for (int n = 1; n <= (SCREEN_WIDTH * SCREEN_HEIGHT / 8) - 1; n++) {
        glcd_buffer[n] = 0;
    }

    // Clear the actual screen
    for (int y = 0; y < 8; y++) {
        glcd_command(GLCD_CMD_SET_PAGE | y);

        // Reset column to 0 (the left side)
        glcd_command(GLCD_CMD_COLUMN_LOWER);
        glcd_command(GLCD_CMD_COLUMN_UPPER);

        // We iterate to 132 as the internal buffer is 65*132, not
        // 64*124.
        for (int x = 0; x < 132; x++) {
            glcd_data(0x00);
        }
    }
}

void glcd_refresh() {
    for (int y = 0; y < 8; y++) {

#ifdef ST7565_DIRTY_PAGES
        // Only copy this page if it is marked as "dirty"
        if (!(glcd_dirty_pages & (1 << y))) continue;
#endif

        glcd_command(GLCD_CMD_SET_PAGE | y);

        // Reset column to the left side.  The internal memory of the
        // screen is 132*64, we need to account for this if the display
        // is flipped.
        //
        // Some screens seem to map the internal memory to the screen
        // pixels differently, the ST7565_REVERSE define allows this to
        // be controlled if necessary.
#ifdef ST7565_REVERSE
        if (!glcd_flipped) {
#else
        if (glcd_flipped) {
#endif
            glcd_command(GLCD_CMD_COLUMN_LOWER | 4);
        } else {
            glcd_command(GLCD_CMD_COLUMN_LOWER);
        }
        glcd_command(GLCD_CMD_COLUMN_UPPER);

        for (int x = 0; x < 128; x++) {
            glcd_data(glcd_buffer[y * 128 + x]);
        }
    }

#ifdef ST7565_DIRTY_PAGES
    // All pages have now been updated, reset the indicator.
    glcd_dirty_pages = 0;
#endif
}

void glcd_init() {
    // The screen is the "normal" way up
    glcd_flipped = 0;
}

void glcd_data(char data) {
    LCD_write_data(data);
}

void glcd_command(char command) {
    LCD_write_command(command);
}

void glcd_flip_screen(unsigned char flip) {
    if (flip) {
        glcd_command(GLCD_CMD_HORIZONTAL_NORMAL);
        glcd_command(GLCD_CMD_VERTICAL_REVERSE);
        glcd_flipped = 0;
    } else {
        glcd_command(GLCD_CMD_HORIZONTAL_REVERSE);
        glcd_command(GLCD_CMD_VERTICAL_NORMAL);
        glcd_flipped = 1;
    }
}

void glcd_inverse_screen(unsigned char inverse) {
    if (inverse) {
        glcd_command(GLCD_CMD_DISPLAY_REVERSE);
    } else {
        glcd_command(GLCD_CMD_DISPLAY_NORMAL);
    }
}

void glcd_test_card() {
    unsigned char p = 0xF0;

    for (int n = 1; n <= (SCREEN_WIDTH * SCREEN_HEIGHT / 8); n++) {
        glcd_buffer[n - 1] = p;

        if (n % 4 == 0) {
            unsigned char q = p;
            p = p << 4;
            p |= q >> 4;
        }
    }

    glcd_refresh();
}

void glcd_contrast(char resistor_ratio, char contrast) {
    if (resistor_ratio > 7 || contrast > 63) return;

    glcd_command(GLCD_CMD_RESISTOR | resistor_ratio);
    glcd_command(GLCD_CMD_VOLUME_MODE);
    glcd_command(contrast);
}
