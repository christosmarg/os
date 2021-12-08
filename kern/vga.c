#include "vga.h"
#include "io.h"

#define VGA_MEM		0xb8000;
#define VGA_COLS	80
#define VGA_ROWS	25
#define VGA_PUTC(c)	(((u_int16_t)vga.color << 8) | (c))

#define CURS_CMD	0x3d4
#define CURS_DATA	0x3d5

struct vga {
	volatile u_int16_t *buf;
	size_t row;
	size_t col;
	u_int8_t color;
};

static struct vga vga;

void
vga_clear(u_int8_t fg, u_int8_t bg)
{
	size_t y, x;

	vga_curs_enable(0x0e, 0x0f); /* Low cursor shape. */
	vga_curs_setpos(0, 0);
	vga.row = 0;
	vga.col = 0;
	vga.buf = (u_int16_t *)VGA_MEM;
	vga_set_color(fg, bg);
	for (x = 0; x < VGA_COLS; x++)
		for (y = 0; y < VGA_ROWS; y++)
			vga.buf[y * VGA_COLS + x] = VGA_PUTC(' ');
}

void
vga_set_color(u_int8_t fg, u_int8_t bg)
{
	vga.color = fg | (bg << 4);
}

void
vga_putc(char c)
{
	switch (c) {
	case '\n':
		vga.row++;
		vga.col = 0;
		break;
	case '\b':
		vga.col--;
		vga.buf[vga.row * VGA_COLS + vga.col] = VGA_PUTC(' ');
		break;
	case '\r':
		vga.col = 0;
		break;
	case '\t':
		vga.col += 4;
		break;
	default:
		vga.buf[vga.row * VGA_COLS + vga.col] = VGA_PUTC(c);
		vga.col++;
	}

	if (vga.row >= VGA_ROWS) {
		/* FIXME: scroll */
		vga_clear(VGA_BLACK, VGA_WHITE);
		vga.row = 0;
		vga.col = 0;
	}
	if (vga.col >= VGA_COLS) {
		vga.row++;
		vga.col = 0;
	}
	vga_curs_setpos(vga.col, vga.row);
}

void
vga_write(const char *str)
{
	while (*str != '\0')
		vga_putc(*str++);
}

/* 
 * The arguments `start` and `end` define the shape of the cursor.
 * The lowest scaline value is 0x00 and the highest 0x0f. A cursor shape
 * with `start = 0x00` and `end = 0x0f` is effectively a block cursor.
 */
void
vga_curs_enable(u_int8_t start, u_int8_t end)
{
	/* 0x0a: Low cursor shape */
	outb(CURS_CMD, 0x0a);
	outb(CURS_DATA, (inb(CURS_DATA) & 0xc0) | start);
	outb(CURS_CMD, 0x0b);
	outb(CURS_DATA, (inb(CURS_DATA) & 0xe0) | end);
}

void
vga_curs_disable(void)
{
	outb(CURS_CMD, 0x0a);
	/* 
	 * Bit representation:
	 * 7    6 | 5	    | 4   0
	 * Unused | Disable | Shape
	 */
	outb(CURS_DATA, 0x20);
}

/* 
 * Returns the position in `y * VGA_COLS + x` format.
 * x = pos % VGA_COLS
 * y = pos / VGA_COLS
 */
u_int16_t
vga_curs_getpos(void)
{
	u_int16_t pos = 0;

	outb(CURS_CMD, 0x0e);
	pos |= (u_int16_t)inb(CURS_DATA) << 8;
	outb(CURS_CMD, 0x0f);
	pos |= inb(CURS_DATA);

	return (pos);
}

void
vga_curs_setpos(int x, int y)
{
	u_int16_t pos = y * VGA_COLS + x;

	outb(CURS_CMD, 0x0e);
	outb(CURS_DATA, (u_int8_t)((pos >> 8) & 0xff));
	outb(CURS_CMD, 0x0f);
	outb(CURS_DATA, (u_int8_t)(pos & 0xff));
}
