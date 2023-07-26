#include "libk.h"

#include "kbd.h"
#include "idt.h"
#include "cpufunc.h"
#include "timer.h"
#include "vga.h"

/*
 * TODO
 *
 * CATCH UP!
 *
 * make kset/unsetenv
 * make device driver framework
 * add a config file (?)
 * fix kbd
 * Makefiles
 * fix pending TODOs, XXXs and FIXMEs
 * remove useless stuff
 */
void
kern_main(void) 
{
	vga_clear(VGA_BLACK, VGA_WHITE);
	idt_init();

	timer_init();
	kbd_init();

	sti();

	/* Off to userland! */
	for (;;);
}
