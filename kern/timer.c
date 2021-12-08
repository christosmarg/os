#include <libc.h>
#include "libk.h"
#include "idt.h"
#include "io.h"

#define TIMER_CMD	0x43
#define TIMER_DATA	0x40
#define SQUARE_WAVE	0x36
#define FREQ		1193180
#define HZ		100

static void timer_callback(struct reg *);

static u_int32_t ticks = 0;

static void
timer_callback(struct reg *r)
{
	ticks++;
	UNUSED(r);
}

void
timer_init(void)
{
	u_int32_t div = FREQ / HZ;

	intr_register_handler(IRQ0, timer_callback);
	/* Repating mode. */
	outb(TIMER_CMD, SQUARE_WAVE);
	outb(TIMER_DATA, (u_int8_t)(div & 0xff));
	outb(TIMER_DATA, (u_int8_t)(div >> 8));
	printf("timer on irq 0\n");

	/*outb(TIMER_CMD, SQUARE_WAVE);*/
	/*outb(TIMER_DATA, 0);*/
	/*outb(TIMER_DATA, 0);*/
}
