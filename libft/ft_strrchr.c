/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:24:02 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 14:44:02 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char	*last_occurence;

	last_occurence = NULL;
	while (*s)
	{
		if (*s == (char)c)
			last_occurence = s;
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return ((char *)last_occurence);
}
/*
#include <stdio.h>

int	main(void)
{
	const char	*str = "Hello, World! Welcome to the World!";
	char		c = 'W';
	char		*result;

	result = ft_strrchr(str, c);
	if (result)
	{
		printf("Last occurence of '%c' is at position : %ld\n", c, result - str);
		printf("Substring from the last occurence : %s\n", result);
	}
	else
	{
		printf("The character '%c' was not fournd.\n", c);
	}
	return (0);
}
*/