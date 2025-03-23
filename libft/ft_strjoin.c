/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 13:00:20 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/23 14:18:16 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	i;
	size_t	j;
	size_t	len_s1;
	size_t	len_s2;

	if (!s1 || !s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	result = (char *)malloc(len_s1 + len_s2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len_s1)
	{
		result[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len_s2)
		result[i++] = s2[j++];
	result[i] = '\0';
	return (result);
}
/*
#include <stdio.h>

int main(void)
{
	char	*s1 = "Hello, ";
	char	*s2 = "world!";
	char	*result;

	result = ft_strjoin(s1, s2);
	printf("Test 1 - Expected: 'Hello, world!' | Result: '%s'\n", result);
	free(result);

	result = ft_strjoin(s1, "");
	printf("Test 2 - Expected: 'Hello, ' | Result: '%s'\n", result);
	free(result);

	result = ft_strjoin("", s2);
	printf("Test 3 - Expected: 'world!' | Result: '%s'\n", result);
	free(result);

	result = ft_strjoin("", "");
	printf("Test 4 - Expected: '' | Result: '%s'\n", result);
	free(result);

	result = ft_strjoin(s1, NULL);
	printf("Test 5 - Expected: NULL | Result: '%s'\n", result);

	result = ft_strjoin(NULL, s2);
	printf("Test 6 - Expected: NULL | Result: '%s'\n", result);

	return (0);
}
*/