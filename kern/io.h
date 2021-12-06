#ifndef _IO_H_
#define _IO_H_

#include <u.h>

static inline u_int8_t
inb(u_int16_t port)
{
	u_int8_t res;

	__asm__ __volatile("in %%dx, %%al" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outb(u_int16_t port, u_int8_t v)
{
	__asm__ __volatile("out %%al, %%dx" : : "a" (v), "d" (port));
}

static inline u_int16_t
inw(u_int16_t port)
{
	u_int16_t res;

	__asm__ __volatile("in %%dx, %%ax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outw(u_int16_t port, u_int16_t v)
{
	__asm__ __volatile("out %%ax, %%dx" : : "a" (v), "d" (port));
}

static inline u_int32_t
inl(u_int16_t port)
{
	u_int32_t res;

	__asm__ __volatile("in %%dx, %%eax" : "=a" (res) : "d" (port));
	return (res);
}

static inline void
outl(u_int16_t port, u_int32_t v)
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

#endif /* _IO_H_ */
