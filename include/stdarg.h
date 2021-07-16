#ifndef _KERNEL_STDARG_H
#define _KERNEL_STDARG_H

#ifndef _VA_LIST_DECLARED
#define _VA_LIST_DECLARED
typedef __va_list va_list;
#endif /* _VA_LIST_DECLARED */

#define va_start(ap, last)	__builtin_va_start((ap), (last))
#define va_arg(ap, type)	__builtin_va_arg((ap), type)
#define va_copy(dest, src)	__builtin_va_copy((dest), (src))
#define va_end(ap)		__builtin_va_end(ap)

#endif /* _KERNEL_STDARG_H */
