/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:46:49 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:28:14 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	gnl_strlen(const char *s)
{
	const char	*ptr;

	if (!s)
		return (0);
	ptr = s;
	while (*ptr)
		ptr++;
	return (ptr - s);
}

char	*gnl_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*gnl_strjoin(char *s1, const char *s2)
{
	char	*str;
	char	*ptr;
	char	*tmp;

	if (!s2)
		return (free_null(s1));
	str = malloc(gnl_strlen(s1) + gnl_strlen(s2) + 1);
	if (!str)
		return (free_null(s1));
	ptr = str;
	if (s1)
	{
		tmp = s1;
		while (*tmp)
			*ptr++ = *tmp++;
	}
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	free(s1);
	return (str);
}

char	*free_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}
