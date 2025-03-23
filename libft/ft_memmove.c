/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 10:19:15 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:32:28 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*d;
	const unsigned char	*s;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (d < s)
	{
		while (len--)
		{
			*d++ = *s++;
		}
	}
	else
	{
		d += len;
		s += len;
		while (len--)
		{
			*(--d) = *(--s);
		}
	}
	return (dst);
}
/*
#include <stdio.h>
#include <string.h>

int main() {
    char str1[20] = "Hello, World!";
    char str2[20] = "Goodbye!";

    // Exemple sans chevauchement
    printf("Avant memmove: %s\n", str1);
    ft_memmove(str1 + 7, str1, 6); // Copie "Hello," à partir de la position 7
    printf("Après memmove: %s\n", str1); // "Hello, Hello!"

    // Exemple avec chevauchement
    printf("Avant memmove: %s\n", str2);
    ft_memmove(str2 + 2, str2, 7); // Chevauchement : "Goodbye!" dans "Goodbye!"
    printf("Après memmove: %s\n", str2); // "GoGoodbye!"

    return 0;
}
*/