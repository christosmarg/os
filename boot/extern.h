#ifndef _KERNEL_EXTERN_H_
#define _KERNEL_EXTERN_H_

#include <stddef.h>
#include <stdint.h>

#include "stdarg.h"
#include "io.h"
#include "tty.h"
#include "string.h"

#define BUFSIZ 1024

#define ARRLEN(x) (sizeof(x) / sizeof((x)[0]))
#define sizeof_field(s, f) (sizeof(((t *)0)->f))

int vsprintf(char *, const char *, va_list);
int sprintf(char *, const char *, ...);
int printf(const char *, ...);

#endif /* _KERNEL_EXTERN_H_ */
