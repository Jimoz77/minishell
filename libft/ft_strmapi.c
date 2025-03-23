/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 13:15:30 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/16 09:35:51 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*new_str;
	size_t	i;
	size_t	len;

	if (!s || !f)
		return (NULL);
	len = 0;
	while (s[len])
		len++;
	new_str = (char *)malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new_str[i] = f(i, s[i]);
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}
/*
#include <stdio.h>

static char	to_upper(unsigned int index, char c)
{
	(void)index;
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}

int	main(void)
{
	char	*result;

	result = ft_strmapi("hello world", to_upper);
	printf("Test 1: '%s'\n", result);
	free(result);

	result = ft_strmapi("", to_upper);
	printf("Test 2: '%s'\n", result);
	free(result);

	result = ft_strmapi("12345", to_upper);
	printf("Test 3: '%s'\n", result);
	free(result);

	return (0);
}
*/