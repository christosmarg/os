#ifndef _LIBC_H_
#define _LIBC_H_

#define NULL			((void *)0)
#define UNUSED(x)		((void)(x))
#define ARRLEN(x)		(sizeof(x) / sizeof(*(x)))
#define CONCAT(x, y)		x ## y
#define offsetof(s, t)		((u_long)(&(((s *)0->t))))

#endif /* _LIBC_H_ */
