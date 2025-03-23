/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:44:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:32:33 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)b;
	i = 0;
	while (i < len)
	{
		ptr[i] = (unsigned char)c;
		i++;
	}
	return (b);
}
/*
#include <stdio.h>

int main(void)
{
    char    str[50] = "Hello, world!";
    printf("Before ft_memset: %s\n", str);

    ft_memset(str, '*', 5);
    printf("After ft_memset: %s\n", str);

    return (0);
}
*/