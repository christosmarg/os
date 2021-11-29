#ifndef _KERNEL_IO_H_
#define _KERNEL_IO_H_

#include <stdint.h>
#include "idt.h"

struct region_descriptor;

static inline void
lidt(struct region_desc *rd)
{
	__asm__ __volatile("lidt (%0)" : : "r" (rd));
}

static inline uint8_t
inb(uint16_t port)
{
	uint8_t res;

	__asm__ __volatile("in %%dx, %%al" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outb(uint16_t port, uint8_t v)
{
	__asm__ __volatile("out %%al, %%dx" : : "a" (v), "d" (port));
}

static inline uint16_t
inw(uint16_t port)
{
	uint16_t res;

	__asm__ __volatile("in %%dx, %%ax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outw(uint16_t port, uint16_t v)
{
	__asm__ __volatile("out %%ax, %%dx" : : "a" (v), "d" (port));
}

static inline uint32_t
inl(uint16_t port)
{
	uint32_t res;

	__asm__ __volatile("in %%dx, %%eax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outl(uint16_t port, uint32_t v)
{
	__asm__ __volatile("out %%eax, %%dx" : : "a" (v), "d" (port));
}

static inline void
hlt(void)
{
	__asm__ __volatile("hlt");
}

static inline void
cli(void)
{
	__asm__ __volatile("cli");
}

static inline void
sti(void)
{
	__asm__ __volatile("sti");
}

#endif /* _KERNEL_IO_H_ */
