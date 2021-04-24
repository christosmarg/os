#include "extern.h"
#include "idt.h"
#include "kbd.h"
#include "timer.h"

void
kernel_main(void) 
{
	tty_init();
	idt_init();
	timer_init(50);
	kbd_init();
	/* Enable interrupts so that the IRQs can work now. */
	__asm__ __volatile__ ("sti");
	tty_write("Nothing to see here yet. At least it booted.\n");
	__asm__ __volatile__ ("hlt");
}
