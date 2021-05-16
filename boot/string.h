#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include <stddef.h>

extern void *memset(void *, int, size_t);
extern void *memcpy(void *, const void *, size_t);
extern size_t strlen(const char *);
extern int strcmp(const char *, const char *);

#endif /* _KERNEL_STRING_H_ */
