/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 20:33:53 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:32:13 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	unsigned char	*ptr1;
	unsigned char	*ptr2;

	ptr1 = (unsigned char *)s1;
	ptr2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (ptr1[i] != ptr2[i])
			return (ptr1[i] - ptr2[i]);
		i++;
	}
	return (0);
}
/*
#include <stdio.h>

int main()
{
    char s1[] = "Hello";
    char s2[] = "HelLo";

    int result = ft_memcmp(s1, s2, 5);
    
    if (result == 0)
        printf("The memory block are equal\n");
    else if (result < 0)
        printf("The first block is smaller\n");
    else
        printf("The first block is larger\n");
    
    if (result != 0)
        printf("The difference between the differing bytes is: %d\n", result);

    return (0);
}
*/