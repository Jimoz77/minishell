/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:26:40 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 12:38:05 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, const char *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	start;
	size_t	end;
	char	*trimmed;

	if (!s1 || !set)
		return (NULL);
	start = 0;
	while (s1[start] && is_in_set(s1[start], set))
		start++;
	end = ft_strlen(s1);
	while (end > start && is_in_set(s1[end - 1], set))
		end--;
	trimmed = (char *)malloc(end - start + 1);
	if (!trimmed)
		return (NULL);
	ft_strlcpy(trimmed, &s1[start], end - start + 1);
	return (trimmed);
}
/*
#include <stdio.h>

int main(void)
{
	char	*result;

	result = ft_strtrim("   Hello World!   ", " ");
	printf("Test 1: '%s'\n", result);
	free(result);

	result = ft_strtrim("Hello World!", " ");
	printf("Test 2: '%s'\n", result);
	free(result);

	result = ft_strtrim("###Hello World!###", "#");
	printf("Test 3: '%s'\n", result);
	free(result);

	result = ft_strtrim("Hello World!", "H");
	printf("Test 4: '%s'\n", result);
	free(result);

	result = ft_strtrim("", " ");
	printf("Test 5: '%s'\n", result);
	free(result);

	result = ft_strtrim("###", "#");
	printf("Test 6: '%s'\n", result);
	free(result);

	result = ft_strtrim("  ##Hello World!##  ", "#"); 
	printf("Test 7: '%s'\n", result);
	free(result);

	result = ft_strtrim("Hello World!", "##"); 
	printf("Test 8: '%s'\n", result ? result : "NULL");

	return (0);
}
*/