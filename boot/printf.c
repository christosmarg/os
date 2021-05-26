#include "extern.h"

static char *
itoa(int n, char *buf, int base)
{
	(void)n;
	(void)base;

	return buf;
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

	return str - buf;
}

int
sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int n;

	va_start(args, fmt);
	n = vsprintf(buf, fmt, args);
	va_end(args);

	return n;
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

	return n;
}
