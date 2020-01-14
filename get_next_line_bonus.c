/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:44:43 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/14 16:21:11 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int			gnl_free_return(char **str1, char **str2, char **str3, int value)
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

static char	*gnl_extractor(char *str, char c)
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
	while (j < i)
	{
		str2[j] = str[j];
		j++;
	}
	str2[j] = '\0';
	return (str2);
}

static int	ft_reader_ret(char **line, char **rest, char **str, ssize_t ret)
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
			ret = BUFFER_SIZE;
		}
	}
	else
		gnl_free_return(rest, line, str, 1);
	free(*str);
	*str = NULL;
	if (ret < BUFFER_SIZE && (*rest == NULL || *rest[0] == '\0'))
		return (0);
	return (1);
}

static int	ft_reader(int fd, char **line, char **rest)
{
	ssize_t	ret;
	char	*str;
	char	*buf;

	if (!(buf = gnl_strdup(" ", BUFFER_SIZE)))
		return (gnl_free_return(rest, &str, &buf, 1));
	if ((ret = read(fd, buf, BUFFER_SIZE)) == -1)
		return (gnl_free_return(rest, &buf, &str, 2));
	buf[ret] = '\0';
	if (!(str = gnl_strjoin(rest, buf, 1, ret)))
		return (gnl_free_return(rest, &buf, &str, 2));
	while (ret == BUFFER_SIZE && gnl_strchr(str, '\n') == NULL)
	{
		if ((ret = read(fd, buf, BUFFER_SIZE)) == -1)
			return (gnl_free_return(rest, &str, &buf, 3));
		if (!(str = gnl_strjoin(&str, buf, 1, ret)))
			return (gnl_free_return(rest, &str, &buf, 3));
	}
	gnl_free_return(&buf, line, rest, 1);
	return (ft_reader_ret(line, rest, &str, ret));
}

int			get_next_line(int fd, char **line)
{
	static char	*rest[256];
	char		*buf;
	ssize_t		ret;
	int			i;

	i = -1;
	if (!(buf = gnl_strdup(" ", 1)))
		return (gnl_free_return(&rest[fd], line, &buf, 1));
	if (fd < 0 || line == NULL || fd > 256 || BUFFER_SIZE < 1
		|| read(fd, buf, 0) == -1)
		return (gnl_free_return(&buf, &rest[fd], line, 1));
	if ((ret = ft_reader(fd, line, &rest[fd])) == -1)
		return (gnl_free_return(&rest[fd], &buf, line, 2));
	gnl_free_return(&buf, line, &rest[fd], 1);
	if (ret == 0)
		gnl_free_return(&rest[fd], &buf, line, 1);
	return (ret);
}
