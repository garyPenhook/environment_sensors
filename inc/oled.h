#ifndef OLED_H
#define OLED_H

#include <stdbool.h>
#include <stdint.h>

/*
 * WeAct Studio 0.96" OLED module V1.0: 128x64 panel, SSD1315 controller, I2C on the
 * shared MSSP1 bus (address 0x3C, write byte 0x78; 0x3D with SB1 bridged).
 *
 * Text only, written straight to the controller's RAM (no frame buffer). Coordinates are
 * a pixel column (0-127) and an 8-pixel page (0-7). Text that would run past column 127
 * is cut off. Characters 0x20-0x7E are printable; 0x7F is a degree sign (OLED_DEG).
 *
 * Every call returns false on a bus error or missing ACK; the display then needs
 * oled_init() again before it is used.
 */

#define OLED_DEG "\x7F"

/**
 * Display off, set orientation, clear RAM, charge pump on, display on (SSD1315 6.9.2).
 * The panel needs >= 20 ms after VDD before this (datasheet t0). Clearing sends ~1 KB:
 * about 38 ms at 333 kHz SCL and FOSC = 16 MHz.
 */
bool oled_init(void);

/** Fill the whole display RAM with 0 (all pixels off). */
bool oled_clear(void);

/** 5x7 font in a 6x8 cell: up to 21 characters per page. */
bool oled_text(uint8_t col, uint8_t page, const char *s);

/** The same font doubled to a 12x16 cell over page and page + 1: up to 10 characters. */
bool oled_text_2x(uint8_t col, uint8_t page, const char *s);

#endif
