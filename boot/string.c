#include "string.h"

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
