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
	if (!(str2 = gnl_strnew(i)))
		return (NULL);
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
	char	buf[BUFFER_SIZE + 1];
	char	*str2;

	gnl_memset(buf, '\0', BUFFER_SIZE + 1);
	ret = read(fd, buf, BUFFER_SIZE);
	//	printf(" rest : %s, ret : %d\n", *rest, ret);
	if ((*rest == NULL || *rest[0] == '\0') && (ret == 0 || ret == -1))
		return (ret);
//	printf("rest : %s\n", *rest);
	//	printf("buf : %s\n", buf);
	if (!(str = gnl_strjoin(rest, buf, 1)))
		return (-1);
//	free(*rest);
//	*rest = NULL;
	//	printf("str : %s\n", str);
	while (ret && gnl_strchr(str, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (!(str = gnl_strjoin(&str, buf, 1)))
			return (-1);
	}
	//	printf("str round 2 : %s\n", str);
	str2 = gnl_strchr(str, '\n');
	if (str2 != NULL)
	{
		if (!(*line = gnl_extractor(str, '\n')))
			return (-1);
		//		printf("str2 in reader : %s", str2);
		if (!(*rest = gnl_strdup(str2)))
			return (-1);
	}
	else
	{
		if (!(*line = gnl_strdup(str)))
			return (-1);
	}
	free(str);
	str = NULL;

	return (1);
}

int	get_next_line(int fd, char **line)
{
	static char	rest[MAX_FD][BUFFER_SIZE + 1];
	char		*rest_alloc;
	char		buf[BUFFER_SIZE + 1];
	int		ret;
	int		i;

	i = -1;
	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE < 1 || line
			== NULL || read(fd, buf, 0) == -1)
		return (-1);
	if (rest[fd] != NULL && rest[fd][0] != '\0')
	{
		if (!(rest_alloc = gnl_strdup((const char *)rest[fd])))
			return (-1);
	}
	else
		rest_alloc = NULL;
//	printf("rest_alloc : %s\n", rest_alloc);
	ret = reader(fd, line, &rest_alloc);
	//	printf("rest_alloc : %s\n", rest_alloc);
	//	printf("ret : %d\n", ret);
	if (rest_alloc != NULL)
	{
		while (rest_alloc[++i])
			rest[fd][i] = rest_alloc[i];
		rest[fd][i] = '\0';
		free(rest_alloc);
		rest_alloc = NULL;
	}
	else
		rest[fd][0] = '\0';
	//	printf("rest_fd : %s\n", rest[fd]);
	return (ret);
}
