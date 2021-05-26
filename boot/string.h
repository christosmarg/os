#ifndef _KERNEL_STRING_H_
#define _KERNEL_STRING_H_

#include <stddef.h>

void *memset(void *, int, size_t);
void *memcpy(void *, const void *, size_t);
size_t strlen(const char *);
int strcmp(const char *, const char *);

#endif /* _KERNEL_STRING_H_ */
