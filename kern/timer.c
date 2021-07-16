#include <sys/libk.h>
#include <sys/idt.h>
#include <sys/port.h>

static void timer_callback(struct reg *);

static uint32_t timer_ticks = 0;

static void
timer_callback(struct reg *r)
{
	timer_ticks++;

	UNUSED(r);
}

void
timer_init(uint32_t hz)
{
	uint32_t div = 1193180 / hz;

	int_add_handler(0, timer_callback);
	outb(P_TIMER_CMD, 0x36);
	outb(P_TIMER_DATA, (uint8_t)(div & 0xff));
	outb(P_TIMER_DATA, (uint8_t)((div >> 8) & 0xff));
}
