#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

//# define BUFFER_SIZE 32
# define MAX_FD 7800

char		*gnl_strchr(const char *s, int c);
char		*gnl_strnew(size_t size);
char		*gnl_strjoin(char **s1, char const *s2, int fr);
char		*gnl_strdup(const char *s1);
void		*gnl_memset(void *b, int c, size_t len);
int		get_next_line(int fd, char **line);
size_t		gnl_strlen(const char *str);

#endif
