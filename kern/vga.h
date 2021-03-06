#ifndef _VGA_H_
#define _VGA_H_

#include <u.h>

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

void vga_clear(u_int8_t, u_int8_t);
void vga_set_color(u_int8_t, u_int8_t);
void vga_putc(char);
void vga_puts(const char *);
void vga_curs_enable(u_int8_t, u_int8_t);
void vga_curs_disable(void);
void vga_curs_setpos(int, int);
u_int16_t vga_curs_getpos(void);

#endif /* _VGA_H_ */
