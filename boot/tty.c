#include "tty.h"

#define _PUTC(c) (((uint16_t)tty.color << 8) | c)
#define _COLOR(f, b) (f | b << 4)

static struct tty_info tty;

void
tty_init(void)
{
	size_t y, x;

	tty.row = 0;
	tty.col = 0;
	tty.color = _COLOR(VGA_GREEN, VGA_BLACK);
	tty.buf = (uint16_t *)0xb8000;
	for (x = 0; x < VGA_COLS; x++)
		for (y = 0; y < VGA_ROWS; y++)
			tty.buf[y * VGA_COLS + x] = _PUTC(' ');
}

void
tty_putc(char c)
{
	switch (c) {
	case '\n':
		tty.row++;
		tty.col = 0;
		break;
	case '\b':
		tty.col--;
		break;
	case '\r':
		tty.col = 0;
		break;
	case '\t':
		tty.col += 8;
		break;
	default:
		tty.buf[tty.row * VGA_COLS + tty.col] = _PUTC(c);
		tty.col++;
	}

	if (tty.row >= VGA_ROWS) {
		tty.row = 0;
		tty.col = 0;
	}
	if (tty.col >= VGA_COLS) {
		tty.row++;
		tty.col = 0;
	}
}

void
tty_write(const char *str)
{
	while (*str != '\0')
		tty_putc(*str++);
}
