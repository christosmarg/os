#include <libc.h>
#include "libk.h"
#include "kbd.h"
#include "idt.h"
#include "io.h"
#include "vga.h"

#define KBD_CMD		0x60
#define KBD_PRESSED	0x80
#define KBD_LSHIFT	0x2a
#define KBD_RSHIFT	0x36
#define KBD_LSHIFT_REL	0xaa
#define KBD_RSHIFT_REL	0xb6

static void kbd_callback(struct reg *);

static u_char kbdus_upper[128] = {
	0,	/* Error */
	27,	/* Escape */
	'!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'{', '}', '\n',
	0,	/* Control */
       	'A', 'S', 'D', 'F', 'G',
	'H', 'J', 'K', 'L', ':', '"', '~', 
	0,	/* Left Shift */
	'|', 'Z', 'X',
	'C', 'V', 'B', 'N', 'M', '<', '>', '?', 
	0,	/* Right Shift */
	'*', 
	0,	/* Alt */
	' ',
	0,	/* Caps Lock */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1-F10 */
	0,	/* Num Lock */
	0,	/* Scroll Lock */
	0,	/* Home Key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* End Key */
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0, 0, 0,
	0, 0,	/* F11, F12 */
	0,	/* The rest are undefined */
};

static u_char kbdus_lower[128] = {
	0,	/* Error */
	27,	/* Escape */
	'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',
	'[', ']', '\n',
	0,	/* Control */
       	'a', 's', 'd', 'f', 'g',
	'h', 'j', 'k', 'l', ';', '\'', '`', 
	0,	/* Left Shift */
	'\\', 'z', 'x',
	'c', 'v', 'b', 'n', 'm', ',', '.', '/', 
	0,	/* Right Shift */
	'*', 
	0,	/* Alt */
	' ',
	0,	/* Caps Lock */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /* F1-F10 */
	0,	/* Num Lock */
	0,	/* Scroll Lock */
	0,	/* Home Key */
	0,	/* Up Arrow */
	0,	/* Page Up */
	'-',
	0,	/* Left Arrow */
	0,
	0,	/* Right Arrow */
	'+',
	0,	/* End Key */
	0,	/* Down Arrow */
	0,	/* Page Down */
	0,	/* Insert Key */
	0,	/* Delete Key */
	0, 0, 0,
	0, 0,	/* F11, F12 */
	0,	/* The rest are undefined */
};

static void
kbd_callback(struct reg *r)
{
	u_int8_t sc;
	int shift = 0;

	if ((sc = inb(KBD_CMD)) & KBD_PRESSED) {
	} else {
		/* TODO: shift */
		if (kbdus_lower[sc] == 'r') {
			dump_regs(r);
			return;
		}
		vga_putc(shift ? kbdus_upper[sc] : kbdus_lower[sc]);
	}
	UNUSED(r);
}

void
kbd_init(void)
{
	intr_register_handler(IRQ1, kbd_callback);
	printf("kbd on irq 1\n");
}
