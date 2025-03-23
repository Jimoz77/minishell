/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 11:03:24 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:33:42 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	int		i;
	int		len;
	char	*copy;

	len = 0;
	while (s1[len])
	{
		len++;
	}
	copy = (char *)malloc((len + 1) * sizeof(char));
	if (!copy)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
/*
#include <stdio.h>

int main(void)
{
    const char *original = "Hello, world!";
    char *duplicate;

    duplicate = ft_strdup(original);

    if (duplicate)
    {
        printf("Original: %s\n", original);
        printf("Duplicate: %s\n", duplicate);
    }
    else
    {
        printf("Memory allocation failed\n");
    }

    free(duplicate);

    return 0;
}
*/