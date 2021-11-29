#include "libk.h"
#include "kbd.h"
#include "idt.h"
#include "page.h"
#include "timer.h"

void
kern_main(void) 
{
	vga_clear(VGA_BLACK, VGA_WHITE);
	idt_init();
	page_init();

	timer_init();
	kbd_init();

	sti();

	/* Off to userland! */
	for (;;);
}
