#include <libc.h>
#include "kbd.h"
#include "idt.h"
#include "io.h"
#include "timer.h"
#include "vm_page.h"
#include "vga.h"

/* TODO: make kset/unsetenv */
/* TODO: make device framework */
/* TODO: add a config */
/* TODO: make sysctl */
void
kern_main(void) 
{
	vga_clear(VGA_BLACK, VGA_WHITE);
	idt_init();
	vm_page_init();

	timer_init();
	kbd_init();

	sti();

	/* Off to userland! */
	for (;;);
}
