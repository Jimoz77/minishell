/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 15:03:29 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:59:44 by lsadikaj         ###   ########.fr       */
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

char	*ft_strtok(char *str, char c)
{
	static char	*save = NULL;
	char		*token_start;

	if (str != NULL)
		save = str;
	else if (save == NULL)
		return (NULL);
	while (*save == c)
		save++;
	if (*save == '\0')
	{
		save = NULL;
		return (NULL);
	}
	token_start = save;
	while (*save != '\0' && *save != c)
		save++;
	if (*save != '\0')
	{
		*save = '\0';
		save++;
	}
	else
		save = NULL;
	return (token_start);
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