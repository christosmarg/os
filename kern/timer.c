#include <sys/libk.h>

#include "idt.h"

static void timer_callback(struct reg *);

static uint32_t timer_ticks = 0;

static void
timer_callback(struct reg *r)
{
	timer_ticks++;
	UNUSED(r);
}

void
timer_init(void)
{
	const uint32_t hz = 60;
	uint32_t div = 1193180 / hz;

	int_add_handler(0, timer_callback);
	outb(IO_TIMER_CMD, 0x36);
	outb(IO_TIMER_DATA, (uint8_t)(div & 0xff));
	outb(IO_TIMER_DATA, (uint8_t)((div >> 8) & 0xff));
	printf("timer on int 0\n");
}
