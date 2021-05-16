#ifndef _KERNEL_TTY_H_
#define _KERNEL_TTY_H_

#include <stddef.h>

#define _VGA_MEM 0xb8000;
#define _VGA_COLS 80
#define _VGA_ROWS 25
#define _PUTC(c) (((uint16_t)tty.color << 8) | (c))

#define VGA_SET_COLOR(fg, bg) ((fg) | (bg) << 4)

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

extern void tty_clear(void);
extern void tty_putc(char);
extern void tty_write(const char *);
extern void tty_curs_enable(uint8_t, uint8_t);
extern void tty_curs_disable(void);
extern void tty_curs_setpos(int, int);
extern uint16_t tty_curs_getpos(void);

#endif /* _KERNEL_TTY_H_ */
