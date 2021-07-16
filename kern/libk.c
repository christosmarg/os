#include <sys/libk.h>
#include <sys/io.h>

static char *
itoa(int n, char *buf, int base)
{
	(void)n;
	(void)base;

	return (buf);
}

void *
memset(void *dst, int v, size_t len)
{
	unsigned char *dst0;
	
	dst0 = dst;
	while (len--)
		*dst0++ = v;

	return (dst);
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

	return (dst);
}

size_t
strlen(const char *str)
{
	const char *s;

	for (s = str; *s != '\0'; s++)
		;	/* nothing */

	return (s - str);
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return (0);

	return (*(const unsigned char *)s1 - *(const unsigned char *)(s2 - 1));
}

int
vsprintf(char *buf, const char *fmt, va_list args)
{
	char *str, *s;
	int base, i, n;

	for (str = buf; *fmt != '\0'; fmt++) {
		base = 10;
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
		if (*fmt == '%') {
			switch (*(++fmt)) {
			case 'c':
				*str++ = (unsigned char)va_arg(args, int);
				continue;
			case 's':
				s = va_arg(args, char *);
				n = strlen(s);
				for (i = 0; i < n; i++)
					*str++ = *s++;
				continue;
			case 'p':
				continue;
			case 'd': /* FALLTHROUGH */
			case 'i':
				n = va_arg(args, int);
				break;
			case 'u':
				n = va_arg(args, unsigned int);
				break;
			case 'o':
				base = 8;
				break;
			case 'X':
				base = 16;
				break;
			case '%':
				*str++ = '%';
				continue;
			}
			str = itoa(n, str, base);
		}
	}
	*str = '\0';

	return (str - buf);
}

int
sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);

	return (n);
}

int
printf(const char *fmt, ...)
{
	char buf[BUFSIZ];
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);
	tty_write(buf);

	return (n);
}

void
panic(const char *fmt, ...)
{
	va_list args;

	cli();
	printf("Kernel panic!\n");
	va_start(args, fmt);
	printf(fmt, args);
	va_end(args);
	hlt();
}
