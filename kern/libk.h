#ifndef _LIBK_H_
#define _LIBK_H_

#include <u.h>

#define BUFSIZ	1024

void	*memset(void *, int, size_t);
void	*memcpy(void *, const void *, size_t);

size_t	strlen(const char *);
size_t	strnlen(const char *, size_t);
size_t	strlcpy(char *, const char *, size_t);
int	strcmp(const char *, const char *);
int	isdigit(char);

int	vsprintf(char *, const char *, va_list);
int	sprintf(char *, const char *, ...);
int	printf(const char *, ...);

void	panic(const char *, ...);

#endif /* _LIBK_H_ */
