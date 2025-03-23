/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 19:27:52 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:32:07 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*ptr;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>
#include <string.h>

int main()
{
    const char  s[] = "Hello, 42!";
    char    c = '4';
    char    *result = ft_memchr(s, c, 10);

    if (result != NULL)
        printf("Character found: %c\n", *result);
    else
        printf("Character not found\n");

    return (0);
}
*/