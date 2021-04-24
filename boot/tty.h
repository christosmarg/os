#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

#define VGA_COLS 80
#define VGA_ROWS 25

struct tty_info {
	volatile uint16_t *buf;
	size_t row;
	size_t col;
	uint8_t color;
};

enum vga_color {
	VGA_BLACK = 0,
	VGA_BLUE,
	VGA_GREEN,
	VGA_CYAN,
	VGA_RED,
	VGA_MAGENTA,
	VGA_BROWN,
	VGA_LIGHT_GREY,
	VGA_DARK_GREY,
	VGA_LIGHT_BLUE,
	VGA_LIGHT_GREEN,
	VGA_LIGHT_CYAN,
	VGA_LIGHT_RED,
	VGA_LIGHT_MAGENTA,
	VGA_LIGHT_BROWN,
	VGA_WHITE,
};

extern void tty_init(void);
extern void tty_putc(char);
extern void tty_write(const char *);

#endif /* _KERNEL_TTY_H */
