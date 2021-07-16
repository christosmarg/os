#include <sys/libk.h>
#include <sys/io.h>
#include <sys/port.h>
#include <sys/tty.h>

#define VGA_MEM 0xb8000;
#define VGA_COLS 80
#define VGA_ROWS 25
#define TTY_PUTC(c) (((uint16_t)tty.color << 8) | (c))

struct tty_info {
	volatile uint16_t *buf;
	size_t row;
	size_t col;
	uint8_t color;
};

static struct tty_info tty;

void
tty_clear(uint8_t fg, uint8_t bg)
{
	size_t y, x;

	tty_curs_enable(0x0e, 0x0f); /* Low cursor shape. */
	tty_curs_setpos(0, 0);
	tty.row = 0;
	tty.col = 0;
	tty.buf = (uint16_t *)VGA_MEM;
	tty_set_color(fg, bg);
	for (x = 0; x < VGA_COLS; x++)
		for (y = 0; y < VGA_ROWS; y++)
			tty.buf[y * VGA_COLS + x] = TTY_PUTC(' ');
}

void
tty_set_color(uint8_t fg, uint8_t bg)
{
	tty.color = fg | (bg << 4);
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
		tty.buf[tty.row * VGA_COLS + tty.col] = TTY_PUTC(' ');
		break;
	case '\r':
		tty.col = 0;
		break;
	case '\t':
		tty.col += 8;
		break;
	default:
		tty.buf[tty.row * VGA_COLS + tty.col] = TTY_PUTC(c);
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
	tty_curs_setpos(tty.col, tty.row);
}

void
tty_write(const char *str)
{
	while (*str != '\0')
		tty_putc(*str++);
}

/* 
 * The arguments `start` and `end` define the shape of the cursor.
 * The lowest scaline value is 0x00 and the highest 0x0f. A cursor shape
 * with `start = 0x00` and `end = 0x0f` is effectively a block cursor.
 */
void
tty_curs_enable(uint8_t start, uint8_t end)
{
	/* 0x0a: Low cursor shape */
	outb(P_CURS_CMD, 0x0a);
	outb(P_CURS_DATA, (inb(P_CURS_DATA) & 0xc0) | start);
	outb(P_CURS_CMD, 0x0b);
	outb(P_CURS_DATA, (inb(P_CURS_DATA) & 0xe0) | end);
}

void
tty_curs_disable(void)
{
	outb(P_CURS_CMD, 0x0a);
	/* 
	 * Bit representation:
	 * 7    6 | 5	    | 4   0
	 * Unused | Disable | Shape
	 */
	outb(P_CURS_DATA, 0x20);
}

/* 
 * Returns the position in `y * VGA_COLS + x` format.
 * x = pos % VGA_COLS
 * y = pos / VGA_COLS
 */
uint16_t
tty_curs_getpos(void)
{
	uint16_t pos = 0;

	outb(P_CURS_CMD, 0x0e);
	pos |= (uint16_t)inb(P_CURS_DATA) << 8;
	outb(P_CURS_CMD, 0x0f);
	pos |= inb(P_CURS_DATA);

	return (pos);
}

void
tty_curs_setpos(int x, int y)
{
	uint16_t pos = y * VGA_COLS + x;

	/* Expect 8 highest bits */
	outb(P_CURS_CMD, 0x0e);
	outb(P_CURS_DATA, (uint8_t)((pos >> 8) & 0xff));
	/* Expect 8 lowest bits */
	outb(P_CURS_CMD, 0x0f);
	outb(P_CURS_DATA, (uint8_t)(pos & 0xff));
}
