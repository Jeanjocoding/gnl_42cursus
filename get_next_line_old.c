/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:44:43 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/07 12:26:50 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

int		check_alloc(int fd, char *rest, char **rest_alloc)
{
	char	buf[BUFFER_SIZE + 1];

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE < 1 || read(fd, buf, 0) == -1)
		return (-1);
	if (rest != NULL && rest[0] != '\0')
	{
		if (!(*rest_alloc = gnl_strdup(rest, 0)))
			return (-1);
	}
	else
		*rest_alloc = NULL;
	return (0);
}

char	*gnl_extractor(char *str, char c)
{
	int		i;
	int		j;
	char	*str2;

	i = 0;
	j = 0;
	while (str[i] != '\0' && str[i] != c)
		i++;
	if (!(str2 = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	gnl_memset(str2, '\0', i + 1);
	while (j < i)
	{
		str2[j] = str[j];
		j++;
	}
	str2[j] = '\0';
	return (str2);
}

int		reader_ret(char **line, char **rest, char ***strz, int ret)
{
//	printf(" strz 0 : %s\n", (*strz)[0]);
	if (!(*line = gnl_extractor((*strz)[0], '\n')))
		return (-1);
//	printf(" line insider reader_ret : %s\n", *line);
	if ((*strz)[1] != NULL && ((*strz)[1])[0] != '\0')
	{
//		printf(" line insider reader_ret : %s\n", line);
//		if (!(*line = gnl_extractor((*strz)[0], '\n')))
//			return (-1);
//		printf(" line insider reader_ret : %s - ", *line);
		if (!(*rest = gnl_strdup((*strz)[1], 0)))
			return (-1);
	}
	else
	{
//		gnl_memset(*rest, '\0', 1);
		*rest = NULL;
//		if (!(*line = gnl_extractor((*strz)[0], '\n')))
//			return (-1);
	}
	if (ret == 0 && *rest == NULL)
		ret = -2;
	free((*strz)[0]);
	(*strz)[0] = NULL;
	free(*strz);
	*strz = NULL;
	if (ret == -2)
		return (0);
	else
		return (1);
}

int		reader(int fd, char **line, char **rest)
{
	int		ret;
	char	**strz;
	char	buf[BUFFER_SIZE + 1];

	if (!(strz = (char**)malloc(sizeof(char*) * 3)))
		return (-1);
	gnl_memset(buf, '\0', BUFFER_SIZE + 1);
	ret = read(fd, buf, BUFFER_SIZE);
	if ((*rest == NULL || *rest[0] == '\0') && (ret == 0 || ret == -1))
	{
//		printf("hello%s\n", "");
		*line = gnl_strdup(" ", 1);;
		return (ret);
	}
//	printf(" first join %s : ", "");
	if (!(strz[0] = gnl_strjoin(rest, buf, 1)))
		return (-1);
	while (ret && gnl_strchr(strz[0], '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		buf[ret] = '\0';
//		printf(" second join %s ", "");
		if (ret != 0)
		{
			if (!(strz[0] = gnl_strjoin(&(strz[0]), buf, 1)))
				return (-1);
		}
	}
	strz[1] = gnl_strdup(gnl_strchr(strz[0], '\n'), 0);
	return (reader_ret(line, rest, &strz, ret));
}

int		get_next_line(int fd, char **line)
{
	static char	rest[MAX_FD][BUFFER_SIZE + 1];
	char		*rest_alloc;
	int			ret;
	int			i;

	i = -1;
	if (check_alloc(fd, (char*)rest[fd], &rest_alloc) == -1 || line == NULL)
		return (-1);
	ret = reader(fd, line, &rest_alloc);
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
	return (ret);
}
