/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:03:29 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 14:59:58 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned char	uc;

	uc = (unsigned char)c;
	while (*s)
	{
		if (*s == uc)
			return ((char *)s);
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return (NULL);
}
/*
int main(void)
{
    const char *str = "Hello, world!";
    char *result;

    result = ft_strchr(str, 'o');
    if (result)
    {
        printf("Found character 'o' at position: %ld\n", result - str);
    }
    else
    {
        printf("Character 'o' not found\n");
    }

    result = ft_strchr(str, 'x');
    if (result)
    {
        printf("Found character 'x' at position: %ld\n", result - str);
    }
    else
    {
        printf("Character 'x' not found\n");
    }

    return 0;
}
*/