/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:44:43 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/07 16:48:08 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

void	gnl_strdel(char **str)
{
	free(*str);
	*str = NULL;
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
		return (-1);
	if (gnl_strchr(*str, '\n') != NULL)
	{
		if (!(*rest = gnl_strdup(gnl_strchr(*str, '\n'), 0)))
			return (-1);
		if (*rest == NULL || *rest[0] == '\0')
		{
			gnl_strdel(rest);
			ret = BUFFER_SIZE;
		}
	}
	else
		gnl_strdel(rest);
		//*rest = NULL;
	free(*str);
	str = NULL;
	if (ret < BUFFER_SIZE && (*rest == NULL || *rest[0] == '\0'))
		return (0);
	return (1);
}

int		reader(int fd, char **line, char **rest)
{
	int		ret;
	char	*str;
//	char	buf[BUFFER_SIZE + 1];
	char	*buf;

	if (!(buf = gnl_strdup(" ", BUFFER_SIZE + 1)))
		return (-1);
//	gnl_memset(buf, '\0', BUFFER_SIZE);
	ret = read(fd, buf, BUFFER_SIZE);
	if (ret == 0 && (*rest == NULL || *rest[0] == '\0'))
	{
		gnl_strdel(&buf);
		gnl_strdel(rest);
		if (!(*line = gnl_strdup(" ", 1)))
			return (-1);
		return (0);
	}
	buf[ret] = '\0';
	if (!(str = gnl_strjoin(rest, buf, 1)))
		return (-1);
//	printf("rest : %s\n", *rest);
//	gnl_memset(buf, '\0', gnl_strlen(buf));
	while (ret == BUFFER_SIZE && gnl_strchr(str, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		buf[ret] = '\0';
		if (!(str = gnl_strjoin(&str, buf, 1)))
			return (-1);
//		gnl_memset(buf, '\0', gnl_strlen(buf));
	}
	gnl_strdel(&buf);
	return (reader_ret(line, rest, &str, ret));
}

int		get_next_line(int fd, char **line)
{
	static char	*rest[MAX_FD];
	char		*buf;
//	char		buf[BUFFER_SIZE + 1];
	int			ret;
	int			i;

	i = -1;
	if (!(buf = gnl_strdup(" ", 1)))
		return (-1);
	if (fd < 0 || line == NULL || fd > MAX_FD || BUFFER_SIZE < 1
		|| read(fd, buf, 0) == -1)
		return (-1);
	ret = reader(fd, line, &rest[fd]);
	gnl_strdel(&buf);
	if (ret == 0)
		gnl_strdel(&rest[fd]);
	return (ret);
}
