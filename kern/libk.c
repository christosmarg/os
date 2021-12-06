#include "libk.h"
#include "io.h"
#include "vga.h"

#define ZEROPAD	0x01
#define SIGN	0x02
#define PLUS	0x04
#define SPACE	0x08
#define LEFT	0x10
#define SMALL	0x20
#define SPECIAL 0x40

static int	calc_width(const char **);
static int	div(long *, int);
static char	*itoa(char *, long, int, int, int, int);

static int
calc_width(const char **s)
{
	int i = 0;

	while (isdigit(**s))
		i = i * 10 + *((*s)++) - '0';
	return (i);
}

static int
div(long *n, int base)
{
	int res;

	res = (u_long)*n % (u_int)base;
	*n = (u_long)*n / (u_int)base;
	return (res);
}

static char *
itoa(char *str, long num, int base, int size, int precision, int flags)
{
	static const char digits[16] = "0123456789ABCDEF";
	char tmp[66];
	int i;
	char c, locase, sign;

	locase = (flags & SMALL);
	if (flags & LEFT)
		flags &= ~ZEROPAD;
	if (base < 2 || base > 16)
		return (NULL);
	c = (flags & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (flags & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (flags & PLUS) {
			sign = '+';
			size--;
		} else if (flags & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (flags & SPECIAL) {
		if (base == 8)
			size--;
		else if (base == 16)
			size -= 2;
	}
	i = 0;
	if (!num)
		tmp[i++] = '0';
	else
		while (num)
			tmp[i++] = (digits[div(&num, base)] | locase);
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(flags & (ZEROPAD | LEFT))) /* XXX: + instead of | ? */
		while (size-- > 0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (flags & SPECIAL) {
		if (base == 8)
			*str++ = '0';
		else if (base == 16) {
			*str++ = '0';
			*str++ = ('X' | locase);
		}
	}
	if (!(flags & LEFT))
		while (size-- > 0)
			*str++ = c;
	while (i < precision--)
		*str++ = '0';
	while (i-- > 0)
		*str++ = tmp[i];
	while (size-- > 0)
		*str++ = ' ';

	return (str);
}

void *
memset(void *dst, int v, size_t len)
{
	u_char *dst0;
	
	dst0 = dst;
	while (len--)
		*dst0++ = v;

	return (dst);
}

void *
memcpy(void *dst, const void *src, size_t len)
{
	const u_char *src0;
	u_char *dst0;

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

size_t
strnlen(const char *str, size_t maxlen)
{
	const char *s = str;

	while (*s && maxlen) {
		s++;
		maxlen--;
	}
	return (s - str);
}

size_t
strlcpy(char *dst, const char *src, size_t dsize)
{
	const char *osrc = src;
	size_t nleft = dsize;

	if (nleft != 0) {
		while (--nleft != 0) {
			if ((*dst++ = *src++) == '\0')
				break;
		}
	}

	if (nleft == 0) {
		if (dsize != 0)
			*dst = '\0';
		while (*src++)
			;
	}

	return (src - osrc - 1);
}

int
strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2++)
		if (*s1++ == '\0')
			return (0);

	return (*(const u_char *)s1 - *(const u_char *)(s2 - 1));
}

int
isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int
vsprintf(char *buf, const char *fmt, va_list ap)
{
	char *str, *s;
	int base, i, n, flags, qual, size, precision;

	for (str = buf; *fmt != '\0'; fmt++) {
		if (*fmt != '%') {
			*str++ = *fmt;
			continue;
		}
		flags = 0;
repeat:
		fmt++;
		switch (*fmt) {
		case '-':
			flags |= LEFT;
			goto repeat;
		case '+':
			flags |= PLUS;
			goto repeat;
		case ' ':
			flags |= SPACE;
			goto repeat;
		case '#':
			flags |= SPECIAL;
			goto repeat;
		case '0':
			flags |= ZEROPAD;
			goto repeat;
		}

		size = -1;
		if (isdigit(*fmt))
			size = calc_width(&fmt);
		else if (*fmt == '*') {
			fmt++;
			if ((size = va_arg(ap, int)) < 0) {
				size -= size;
				flags |= LEFT;
			}
		}

		precision = -1;
		if (*fmt == '.') {
			fmt++;
			if (isdigit(*fmt))
				precision = calc_width(&fmt);
			else if (*fmt == '*') {
				fmt++;
				precision = va_arg(ap, int);
			}
			if (precision < 0)
				precision = 0;
		}

		qual = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
			qual = *fmt++;

		base = 10;
		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
				while (--size > 0)
					*str++ = ' ';
			*str++ = (u_char)va_arg(ap, int);
			while (--size > 0)
				*str++ = ' ';
			continue;
		case 's':
			s = va_arg(ap, char *);
			n = strnlen(s, precision);
			if (!(flags & LEFT))
				while (n < size--)
					*str++ = ' ';
			for (i = 0; i < n; i++)
				*str++ = *s++;
			while (n < size--)
				*str++ = ' ';
			continue;
		case 'p':
			if (size == -1) {
				size = sizeof(void *) << 1;
				flags |= ZEROPAD;
			}
			str = itoa(str,
			    (u_long)va_arg(ap, void *), 16,
			    size, precision, flags);
			continue;
		case 'n':
			if (qual == 'l') {
				long *ip = va_arg(ap, long *);
				*ip = (str - buf);
			} else {
				int *ip = va_arg(ap, int *);
				*ip = (str - buf);
			}
			continue;
		case '%':
			*str++ = '%';
			continue;
		case 'd': /* FALLTHROUGH */
		case 'i':
			flags |= SIGN;
			break;
		case 'u':
			break;
		case 'o':
			base = 8;
			break;
		case 'x': /* FALLTHROUGH */
			flags |= SMALL;
		case 'X':
			base = 16;
			break;
		default:
			*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				fmt--;
			continue;
		}

		if (qual == 'l')
			n = va_arg(ap, u_long);
		else if (qual == 'h') {
			n = (u_short)va_arg(ap, int);
			if (flags & SIGN)
				n = (short)n;
		} else if (flags & SIGN)
			n = va_arg(ap, int);
		else
			n = va_arg(ap, u_int);
		str = itoa(str, n, base, size, precision, flags);
	}
	*str = '\0';

	return (str - buf);
}

int
sprintf(char *buf, const char *fmt, ...)
{
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsprintf(buf, fmt, ap);
	va_end(ap);

	return (n);
}

int
printf(const char *fmt, ...)
{
	char buf[BUFSIZ];
	va_list ap;
	int n;

	va_start(ap, fmt);
	n = vsprintf(buf, fmt, ap);
	va_end(ap);
	vga_write(buf);

	return (n);
}

void
panic(const char *fmt, ...)
{
	char buf[BUFSIZ];
	va_list ap;
	int n;

	cli();
	vga_set_color(VGA_RED, VGA_WHITE);
	printf("panic: ");
	va_start(ap, fmt);
	n = vsprintf(buf, fmt, ap);
	va_end(ap);
	vga_write(buf);
	vga_set_color(VGA_BLACK, VGA_WHITE);
	hlt();
}
