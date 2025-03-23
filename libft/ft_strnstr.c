/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 09:15:51 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:34:17 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	if (*needle == '\0')
		return ((char *)haystack);
	i = 0;
	while (haystack[i] && i < len)
	{
		j = 0;
		while (haystack [i + j] && needle [j] && (i + j) < len
			&& haystack[i + j] == needle[j])
			j++;
		if (needle[j] == '\0')
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
/*
#include <stdio.h>

int main()
{
    const char haystack[] = "Hello, this is a simple string";
    const char needle[] = "simple";
    size_t len = 31;

    char *result = ft_strnstr(haystack, needle, len);
    
    if (result)
        printf("Found: %s\n", result);
    else
        printf("Not Found\n");

    return 0;
}
*/