#include <sys/libk.h>
#include <sys/idt.h>
#include <sys/io.h>
#include <sys/timer.h>

#include <dev/kbd.h>

#define OK() ((void)printf("ok\n"))

void
kern_main(void) 
{
	tty_clear(VGA_BLUE, VGA_WHITE);

	printf("IDT... ");
	idt_init();
	OK();

	printf("Timer... ");
	timer_init(50);
	OK();

	printf("Keyboard... ");
	kbd_init();
	OK();

	sti();
	for (;;);
}
