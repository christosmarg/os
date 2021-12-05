#include <libc.h>
#include "libk.h"
#include "idt.h"
#include "io.h"

#define TIMER_CMD	0x43
#define TIMER_DATA	0x40

static void timer_callback(struct reg *);

static u_int32_t timer_ticks = 0;

static void
timer_callback(struct reg *r)
{
	timer_ticks++;
	UNUSED(r);
}

void
timer_init(void)
{
	const u_int32_t hz = 100;
	u_int32_t div = 1193180 / hz;

	intr_register_handler(IRQ0, timer_callback);
	/* Repating mode. */
	outb(TIMER_CMD, 0x36);
	outb(TIMER_DATA, (u_int8_t)(div & 0xff));
	outb(TIMER_DATA, (u_int8_t)((div >> 8) & 0xff));
	printf("timer on irq 0\n");
}
