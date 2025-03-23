/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 13:34:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/16 13:15:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
/*
#include <stdio.h>
#include <ctype.h>

satic void	to_upper(unsigned int index, char *c)
{
	(void)index;
	*c = (char)toupper(*c);
}

int	main(void)
{
	char	str[] = "hello world";
	ft_striteri(str, to_upper);
	printf("Modified string: '%s'\n", str); // Should print 'HELLO WORLD'

	return (0);
}
*/