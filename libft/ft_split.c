/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:32:25 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/14 20:03:00 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *s, char c)
{
	int		count;
	int		in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (count);
}

static char	*word_dup(const char *start, int len)
{
	char	*word;
	int		i;

	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_split(char **split, int i)
{
	while (i >= 0)
		free(split[i--]);
	free(split);
}

static char	**init_split(char **split, char *s, int c)
{
	int	i;
	int	len;

	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = 0;
			while (s[len] && s[len] != c)
				len++;
			split[i] = word_dup(s, len);
			if (!split[i])
				return (free_split(split, i), NULL);
			i++;
			s += len;
		}
		else
			s++;
	}
	split[i] = NULL;
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;

	if (!s)
		return (NULL);
	split = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!split)
		return (NULL);
	return (init_split(split, (char *)s, c));
}

char    *ft_strtok(char *str, char c)
{
    static char *save = NULL;
    char        *token_start;

    // Réinitialisation ou continuation de la chaîne
    if (str != NULL)
        save = str;
    else if (save == NULL)
        return (NULL);

    // Ignorer les délimiteurs au début
    while (*save == c)
        save++;

    // Fin de la chaîne -> retourner NULL
    if (*save == '\0')
    {
        save = NULL;
        return (NULL);
    }

    token_start = save;

    // Trouver la fin du token
    while (*save != '\0' && *save != c)
        save++;

    // Remplacer le délimiteur par '\0' et avancer
    if (*save != '\0')
    {
        *save = '\0';
        save++;
    }
    else
        save = NULL;

    return (token_start);
}