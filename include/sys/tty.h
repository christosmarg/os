#ifndef _KERNEL_TTY_H_
#define _KERNEL_TTY_H_

#include <stddef.h>

enum vga_color: uint8_t {
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

void tty_clear(uint8_t, uint8_t);
void tty_set_color(uint8_t, uint8_t);
void tty_putc(char);
void tty_write(const char *);
void tty_curs_enable(uint8_t, uint8_t);
void tty_curs_disable(void);
void tty_curs_setpos(int, int);
uint16_t tty_curs_getpos(void);

#endif /* _KERNEL_TTY_H_ */
