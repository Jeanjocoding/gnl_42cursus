/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:44:43 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/10 18:02:43 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

int		gnl_free_return(char **str1, char **str2, char **str3, int value)
{
	if (value > 0)
	{
		free(*str1);
		*str1 = NULL;
	}
	if (value > 1)
	{
		free(*str2);
		*str2 = NULL;
	}
	if (value > 2)
	{
		free(*str3);
		*str3 = NULL;
	}
	return (-1);
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

int		reader_ret(char **line, char **rest, char **str, int ret)
{
	if (!(*line = gnl_extractor(*str, '\n')))
		return (gnl_free_return(str, rest, line, 1));
	if (gnl_strchr(*str, '\n') != NULL)
	{
		if (!(*rest = gnl_strdup(gnl_strchr(*str, '\n'), 0)))
			return (gnl_free_return(str, line, rest, 1));
		if (*rest == NULL || *rest[0] == '\0')
		{
			gnl_free_return(rest, line, str, 1);
			ret = (int)BUFFER_SIZE;
		}
	}
	else
		gnl_free_return(rest, line, str, 1);
	free(*str);
	*str = NULL;
	if (ret < (int)BUFFER_SIZE && (*rest == NULL || *rest[0] == '\0'))
		return (0);
	return (1);
}

int		reader(int fd, char **line, char **rest)
{
	int		ret;
	char	*str;
	char	*buf;

	if (!(buf = gnl_strdup(" ", (int)BUFFER_SIZE)))
		return (gnl_free_return(rest, &str, &buf, 1));
	if ((ret = read(fd, buf, BUFFER_SIZE)) == -1)
		return (gnl_free_return(rest, &buf, &str, 2));
	if (ret == 0 && (*rest == NULL || *rest[0] == '\0'))
	{
		gnl_free_return(&buf, line, rest, 1);
		if (!(*line = gnl_strdup(" ", 1)))
			return (gnl_free_return(rest, &str, &buf, 1));
		return (0);
	}
	buf[ret] = '\0';
	if (!(str = gnl_strjoin(rest, buf, 1, ret)))
		return (gnl_free_return(rest, &buf, &str, 2));
	while (ret == (int)BUFFER_SIZE && gnl_strchr(str, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (!(str = gnl_strjoin(&str, buf, 1, ret)))
			return (gnl_free_return(rest, &str, &buf, 3));
	}
	gnl_free_return(&buf, line, rest, 1);
	return (reader_ret(line, rest, &str, ret));
}

int		get_next_line(int fd, char **line)
{
	static char	*rest[MAX_FD];
	char		*buf;
	int			ret;
	int			i;

	i = -1;
	if (!(buf = gnl_strdup(" ", 1)))
		return (gnl_free_return(&rest[fd], line, &buf, 1));
	if (fd < 0 || line == NULL || fd > MAX_FD || (int)BUFFER_SIZE < 1
		|| read(fd, buf, 0) == -1)
		return (gnl_free_return(&buf, &rest[fd], line, 1));
	if ((ret = reader(fd, line, &rest[fd])) == -1)
		return (gnl_free_return(&rest[fd], &buf, line, 3));
	gnl_free_return(&buf, line, &rest[fd], 1);
	if (ret == 0)
		gnl_free_return(&rest[fd], &buf, line, 1);
	return (ret);
}
