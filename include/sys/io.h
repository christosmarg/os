#ifndef _KERNEL_IO_H_
#define _KERNEL_IO_H_

#include <stdint.h>

#define IO_PIC1_CMD	0x20
#define IO_PIC2_CMD	0xa0
#define IO_PIC1_DATA	(IO_PIC1_CMD + 1)
#define IO_PIC2_DATA	(IO_PIC2_CMD + 1)
#define IO_TIMER_CMD	0x43
#define IO_TIMER_DATA	0x40
#define IO_CURS_CMD	0x3d4
#define IO_CURS_DATA	0x3d5
#define IO_KBD		0x60

static inline uint8_t
inb(uint16_t port)
{
	uint8_t res;

	__asm__ __volatile__ ("in %%dx, %%al" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outb(uint16_t port, uint8_t v)
{
	__asm__ __volatile__ ("out %%al, %%dx" : : "a" (v), "d" (port));
}

static inline uint16_t
inw(uint16_t port)
{
	uint16_t res;

	__asm__ __volatile__ ("in %%dx, %%ax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outw(uint16_t port, uint16_t v)
{
	__asm__ __volatile__ ("out %%ax, %%dx" : : "a" (v), "d" (port));
}

static inline uint32_t
inl(uint16_t port)
{
	uint32_t res;

	__asm__ __volatile__ ("in %%dx, %%eax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outl(uint16_t port, uint32_t v)
{
	__asm__ __volatile__ ("out %%eax, %%dx" : : "a" (v), "d" (port));
}

static inline void
hlt(void)
{
	__asm__ __volatile__ ("hlt");
}

static inline void
cli(void)
{
	__asm__ __volatile__ ("cli");
}

static inline void
sti(void)
{
	__asm__ __volatile__ ("sti");
}

#endif /* _KERNEL_IO_H_ */
