/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:44:43 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/06 17:12:26 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int		check_alloc(int fd, char *rest, char **rest_alloc)
{
	char	buf[BUFFER_SIZE + 1];

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE < 1 || read(fd, buf, 0) == -1)
		return (-1);
	if (rest != NULL && rest[0] != '\0')
	{
		if (!(*rest_alloc = gnl_strdup(rest)))
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
	{
		i++;
	}
	if (!(str2 = (char*)malloc(sizeof(char) * (i + 1))))
		return (NULL);
	while (j < i)
	{
		str2[j] = str[j];
		j++;
	}
	str2[j] = '\0';
	return (str2);
}

int		reader_ret(char **line, char **rest, char **str, char *str2)
{
	if (str2 != NULL)
	{
		if (!(*line = gnl_extractor(*str, '\n')))
			return (-1);
		if (!(*rest = gnl_strdup(str2)))
			return (-1);
	}
	else
	{
		if (!(*line = gnl_strdup(*str)))
			return (-1);
	}
	free(*str);
	*str = NULL;
	return (1);
}

int		reader(int fd, char **line, char **rest)
{
	int		ret;
	char	*str;
	char	buf[BUFFER_SIZE + 1];
	char	*str2;

	gnl_memset(buf, '\0', BUFFER_SIZE + 1);
	ret = read(fd, buf, BUFFER_SIZE);
	if ((*rest == NULL || *rest[0] == '\0') && (ret == 0 || ret == -1))
	{
		*line = NULL;
		return (ret);
	}
	if (!(str = gnl_strjoin(rest, buf, 1)))
		return (-1);
	while (ret && gnl_strchr(str, '\n') == NULL)
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (!(str = gnl_strjoin(&str, buf, 1)))
			return (-1);
	}
	str2 = gnl_strchr(str, '\n');
	return (reader_ret(line, rest, &str, str2));
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
