#include "extern.h"
#include "idt.h"
#include "kbd.h"
#include "timer.h"

void
kmain(void) 
{
	tty_clear();
	idt_init();
	timer_init(50);
	kbd_init();
	__asm__ __volatile__ ("sti");

	for (;;);
}
