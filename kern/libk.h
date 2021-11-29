#ifndef _KERNEL_LIBK_H_
#define _KERNEL_LIBK_H_

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "io.h"
#include "vga.h"

#define BUFSIZ 1024

#define ARRLEN(x)		(sizeof(x) / sizeof(*(x)))
#define UNUSED(x)		((void)(x))
#define sizeof_field(s, f)	(sizeof(((t *)0)->f))

void	*memset(void *, int, size_t);
void	*memcpy(void *, const void *, size_t);
size_t	strlen(const char *);
size_t	strnlen(const char *, size_t);
int	strcmp(const char *, const char *);
int	isdigit(char);
int	vsprintf(char *, const char *, va_list);
int	sprintf(char *, const char *, ...);
int	printf(const char *, ...);
void	panic(const char *, ...);

#endif /* _KERNEL_LIBK_H_ */
