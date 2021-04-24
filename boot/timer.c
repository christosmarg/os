#include "extern.h"
#include "idt.h"

static void timer_callback(struct reg *);

static uint32_t timer_ticks = 0;

static void
timer_callback(struct reg *r)
{
	timer_ticks++;
	(void)r;
}

void
timer_init(uint32_t hz)
{
	uint32_t div = 1193180 / hz;

	irq_add_handler(0, timer_callback);
	outb(0x36, 0x43);
	outb((uint8_t)(div & 0xff), 0x40);
	outb((uint8_t)((div >> 8) & 0xff), 0x40);
}
