#include "string.h"

void *
memset(void *dst, int v, size_t len)
{
	unsigned char *dst0;
	
	dst0 = dst;
	while (len--)
		*dst0++ = v;
	return dst;
}

void *
memcpy(void *dst, const void *src, size_t len)
{
	const unsigned char *src0;
	unsigned char *dst0;

	src0 = src;
	dst0 = dst;

	while (len--)
		*dst0++ = *src0++;
	return dst;
}

size_t
strlen(const char *str)
{
	const char *s;

	for (s = str; *s != '\0'; s++)
		;
	return (s - str);
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return 0;
	return *(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1);
}
