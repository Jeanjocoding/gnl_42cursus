/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucille <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/06 12:49:34 by tlucille          #+#    #+#             */
/*   Updated: 2020/01/07 16:48:23 by tlucille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_strlen(const char *str)
{
	unsigned int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*gnl_strchr(const char *s, int c)
{
	unsigned char	c2;
	unsigned int	i;
	unsigned int	len;

	i = 0;
	c2 = c;
	len = gnl_strlen(s);
	while (i <= len)
	{
		if (s[i] == c2)
			return ((char*)&s[i + 1]);
		i++;
	}
	return (NULL);
}

char	*gnl_strjoin(char **s1, char const *s2, int fr)
{
	unsigned int	i;
	unsigned int	j;
	char			*scat;

	i = 0;
	j = 0;
	if (*s1 == NULL && s2 != NULL)
		return (gnl_strdup(s2, 0));
	if (*s1 != NULL && s2 == NULL)
		return (gnl_strdup(*s1, 0));
	if (!(scat = (char*)malloc(sizeof(char) *
		(gnl_strlen(*s1) + gnl_strlen(s2)) + 1)))
		return (NULL);
	while ((*s1)[i] != '\0')
		scat[j++] = (*s1)[i++];
	i = 0;
	while (s2[i])
		scat[j++] = s2[i++];
	scat[j] = '\0';
	if (fr == 1)
	{
		free(*s1);
		*s1 = NULL;
	}
	return (scat);
}

char	*gnl_strdup(const char *s1, int empty)
{
	unsigned int	i;
	unsigned int	len;
	char			*mlc;

	i = 0;
	if (empty > 0)
	{
		if (!(mlc = (char*)malloc(sizeof(char) * (empty))))
			return (NULL);
		gnl_memset(mlc, '\0', empty);
		return (mlc);
	}
	if (s1 == NULL)
		return (NULL);
	len = gnl_strlen(s1);
	if (!(mlc = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	while (i < len)
	{
		mlc[i] = s1[i];
		i++;
	}
	mlc[i] = '\0';
	return (mlc);
}

void	*gnl_memset(void *b, int c, size_t len)
{
	unsigned int	i;
	unsigned char	*strz;

	i = 0;
	strz = (unsigned char*)b;
	while (i < len)
	{
		strz[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
