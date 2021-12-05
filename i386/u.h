#ifndef _U_H_
#define _U_H_

#include <stddef.h> /* XXX: keep for now... */

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned int		u_int;
typedef unsigned long		u_long;
typedef unsigned long long	u_vlong;
typedef long long		vlong;

typedef signed char		int8_t;
typedef unsigned char		u_int8_t;
typedef unsigned short		u_int16_t;
typedef short			int16_t;
typedef unsigned int		u_int32_t;
typedef int			int32_t;
typedef unsigned long		u_int64_t;
typedef long			int64_t;

//typedef u_int64_t		size_t;

typedef __va_list		va_list;
#define va_start(ap, last)	__builtin_va_start((ap), (last))
#define va_arg(ap, type)	__builtin_va_arg((ap), type)
#define va_copy(dest, src)	__builtin_va_copy((dest), (src))
#define va_end(ap)		__builtin_va_end(ap)

#endif /* _U_H_ */
