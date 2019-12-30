#include "get_next_line.h"
#include <stdio.h>

size_t		gnl_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*gnl_extractor(char *str, char c)
{
	int	i;
	int	j;
	char 	*str2;

	i = 0;
	j = 0;
//	printf("str in extractor : %s", str);
	while (str[i] != '\0' && str[i] != c)
	{
	//	printf("%d", i);
		i++;
	}
	str2 = gnl_strnew(i);
	while (j < i)
	{
		str2[j] = str[j];
		j++;
	}
	return (str2);
}

int	reader(int fd, char **line, char **rest)
{
	int	ret;
	char	*str;
	char	buf[BUF_SIZE + 1];
	char	*str2;

	gnl_memset(buf, '\0', BUF_SIZE + 1);
	ret = read(fd, buf, BUF_SIZE);
//	printf(" rest : %s, ret : %d\n", *rest, ret);
	if ((*rest == NULL || *rest[0] == '\0') && (ret == 0 || ret == -1))
		return (ret);
//	printf("rest : %s\n", *rest);
//	printf("buf : %s\n", buf);

	str = gnl_strjoin(*rest, buf);
	free(*rest);
	*rest = NULL;
//	printf("str : %s\n", str);
	while (ret && gnl_strchr(str, '\n') == NULL)
	{
		ret = read(fd, buf, BUF_SIZE);
		str = gnl_strjoin(str, buf);
	}
//	printf("str round 2 : %s\n", str);
	str2 = gnl_strchr(str, '\n');
	if (str2 != NULL)
	{
		*line = gnl_extractor(str, '\n');
//		printf("str2 in reader : %s", str2);
		*rest = gnl_strdup(str2);
	}
	else
	{
		*line = gnl_strdup(str);
	//	*rest = NULL;
	}
		
	return (1);
}

int	get_next_line(int fd, char **line)
{
	static char	*rest[MAX_FD];
	char		*rest_alloc;
	int		ret;

	if (rest[fd] != NULL)
		rest_alloc = gnl_strdup(rest[fd]);
	else
		rest_alloc = (void*)0;
//	printf("rest_alloc : %s\n", rest_alloc);
	ret = reader(fd, line, &rest_alloc);
//	printf("rest_alloc : %s\n", rest_alloc);
//	printf("ret : %d\n", ret);
	if (rest_alloc != NULL)
		rest[fd] = gnl_strdup(rest_alloc);
	else
		rest[fd] = NULL;
//	printf("rest_fd : %s\n", rest[fd]);
	return (ret);
}
