#include <sys/libk.h>

#include <dev/kbd.h>

#include "idt.h"
#include "timer.h"

int
kern_main(void) 
{
	tty_clear(VGA_BLUE, VGA_WHITE);
	idt_init();
	timer_init();
	kbd_init();
	sti();

	/* Off to userland! */
	for (;;);

	/* NOTREACHED */
	return (0);
}
