#include "libk.h"
#include "idt.h"

#define TIMER_CMD	0x43
#define TIMER_DATA	0x40

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

	intr_register_handler(0, timer_callback);
	outb(TIMER_CMD, 0x36);
	outb(TIMER_DATA, (uint8_t)(div & 0xff));
	outb(TIMER_DATA, (uint8_t)((div >> 8) & 0xff));
	printf("timer on irq %d\n", 0);
}
