#ifndef _KERNEL_LIBK_H_
#define _KERNEL_LIBK_H_

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include <sys/io.h>
#include <sys/tty.h>

#define BUFSIZ 1024

#define ARRLEN(x)		(sizeof(x) / sizeof((x)[0]))
#define UNUSED(x)		((void)(x))
#define sizeof_field(s, f)	(sizeof(((t *)0)->f))

void *memset(void *, int, size_t);
void *memcpy(void *, const void *, size_t);
size_t strlen(const char *);
int strcmp(const char *, const char *);
int vsprintf(char *, const char *, va_list);
int sprintf(char *, const char *, ...);
/* TODO: kprintf */
int printf(const char *, ...);
void panic(const char *, ...);

#endif /* _KERNEL_LIBK_H_ */
