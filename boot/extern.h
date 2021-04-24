#ifndef _KERNEL_EXTERN_H
#define _KERNEL_EXTERN_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include "tty.h"
#include "string.h"

static inline uint8_t
inb(uint16_t port)
{
	uint8_t res;

	__asm__ __volatile__ ("in %%dx, %%al" : "=a" (res) : "d" (port));
	return res;
}

static inline void
outb(uint8_t v, uint16_t port)
{
	__asm__ __volatile__ ("out %%al, %%dx" : : "a" (v), "d" (port));
}

static inline uint16_t
inw(uint16_t port)
{
	uint16_t res;

	__asm__ __volatile__ ("in %%dx, %%ax" : "=a" (res) : "d" (port));
	return res;
}

static inline void
outw(uint16_t v, uint16_t port)
{
	__asm__ __volatile__ ("out %%ax, %%dx" : : "a" (v), "d" (port));
}

static inline uint32_t
inl(uint16_t port)
{
	uint32_t res;

	__asm__ __volatile__ ("in %%dx, %%eax" : "=a" (res) : "d" (port));
	return res;
}

static inline void
outl(uint32_t v, uint16_t port)
{
	__asm__ __volatile__ ("out %%eax, %%dx" : : "a" (v), "d" (port));
}

#endif /* _KERNEL_EXTERN_H */
