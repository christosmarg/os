#include "extern.h"
#include "idt.h"
#include "port.h"

static void kbd_callback(struct reg *);

static unsigned char kbdus[128] = {
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
	uint8_t sc;

	if ((sc = inb(P_KBD)) & 0x80) {
	} else {
		tty_putc(kbdus[sc]);
	}
	(void)r;
}

/* 
 * Send `kbd_callback` to the `irq_routines` array to be called
 * for IRQ 1.
 */
void
kbd_init(void)
{
	int_add_handler(1, kbd_callback);
}
