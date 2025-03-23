/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 03:46:49 by anpayot           #+#    #+#             */
/*   Updated: 2025/02/12 10:27:31 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/**
 * @brief Calculate length of null-terminated string
 * 
 * @param s String to measure
 * @return size_t Number of characters before null terminator
 */
size_t	gnl_strlen(const char *s)
{
	const char	*ptr;

	if (!s)
		return (0);
	ptr = s;
	while (*ptr)
		ptr++;
	return (ptr - s);
}

/**
 * @brief Locate first occurrence of character in string
 * 
 * @param s String to search
 * @param c Character to find (converted to unsigned char)
 * @return char* Pointer to first occurrence, or NULL if not found
 */
char	*gnl_strchr(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	return (NULL);
}

/**
 * @brief Concatenate two strings into new allocation
 * 
 * @param s1 First string (will be freed)
 * @param s2 Second string
 * @return char* New allocated string with s1+s2, or NULL if allocation fails
 * @note Frees s1 before returning
 */
char	*gnl_strjoin(char *s1, const char *s2)
{
	char	*str;
	char	*ptr;
	char	*tmp;

	if (!s2)
		return (free_null(s1));
	str = malloc(gnl_strlen(s1) + gnl_strlen(s2) + 1);
	if (!str)
		return (free_null(s1));
	ptr = str;
	if (s1)
	{
		tmp = s1;
		while (*tmp)
			*ptr++ = *tmp++;
	}
	while (*s2)
		*ptr++ = *s2++;
	*ptr = '\0';
	free(s1);
	return (str);
}

/**
 * @brief Free pointer and return NULL
 * 
 * @param ptr Pointer to free
 * @return char* Always returns NULL
 * @note Safe to call with NULL pointer
 */
char	*free_null(void *ptr)
{
	if (ptr)
		free(ptr);
	return (NULL);
}

/*
#include <stdio.h>
#include <strings.h>

int main(void)
{
	char *str1 = "Hello, ";
	char *str2 = "World!";
	char *joined_str;
	char *found_char;
	size_t len;

	// Test ft_strlen
	len = ft_strlen(str1);
	printf("Length of '%s': %zu\n", str1, len);

	// Test ft_strchr
	found_char = ft_strchr(str1, 'e');
	if (found_char)
		printf("Char 'e' found in '%s' at pos: %ld\n", str1, found_char - str1);
	else
		printf("Character 'e' not found in '%s'\n", str1);

	// Test ft_strjoin
	joined_str = ft_strjoin(strdup(str1), str2);
	if (joined_str)
	{
		printf("Joined string: '%s'\n", joined_str);
		free(joined_str);
	}
	else
	{
		printf("Failed to join strings\n");
	}

	// Test free_null
	char *ptr = malloc(10);
	if (ptr)
	{
		ptr = free_null(ptr);
		if (!ptr)
			printf("Pointer successfully freed and set to NULL\n");
		else
			printf("Pointer not set to NULL\n");
	}

	return 0;
}
*/