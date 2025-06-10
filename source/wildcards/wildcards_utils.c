/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:18:10 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:36:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Compte le nombre de fichiers dans le répertoire courant qui correspondent
// au motif de wildcard. Ignore les fichiers cachés
// sauf si le motif commence par '.'
int	count_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (match_pattern(pattern, entry->d_name))
			count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (count);
}

// Fonction auxiliaire pour remplir le tableau de correspondances.
static int	fill_matches(char **matches, const char *pattern,
					int count, DIR *dir)
{
	struct dirent	*entry;
	int				i;
	int				ret;

	i = 0;
	entry = readdir(dir);
	while (entry && i < count)
	{
		ret = add_if_match(matches, i, pattern, entry);
		if (ret == -1)
		{
			free_matches_partial(matches, i, dir);
			return (0);
		}
		if (ret == 1)
			i++;
		entry = readdir(dir);
	}
	matches[i] = NULL;
	return (1);
}

// Alloue et remplit un tableau de fichiers correspondant au motif donné.
char	**get_matching_files(const char *pattern, int count)
{
	DIR				*dir;
	char			**matches;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		free(matches);
		return (NULL);
	}
	if (!fill_matches(matches, pattern, count, dir))
		return (NULL);
	closedir(dir);
	return (matches);
}

// Trie un tableau de chaînes par ordre alphabétique en utilisant l'algorithme
// du tri à bulles. Compare les chaînes avec ft_strncmp.
void	sort_strings(char **strings, int count)
{
	int		i;
	int		j;
	char	*temp;
	int		max_len;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strlen(strings[j]) > ft_strlen(strings[j + 1]))
				max_len = ft_strlen(strings[j]);
			else
				max_len = ft_strlen(strings[j + 1]);
			if (ft_strncmp(strings[j], strings[j + 1], max_len) > 0)
			{
				temp = strings[j];
				strings[j] = strings[j + 1];
				strings[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

// Crée un nouveau token de type WORD à partir d'une chaîne de caractères.
// Alloue la mémoire nécessaire et initialise les champs.
t_token	*create_match_token(char *value)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->type = TOKEN_WORD;
	new->parts = NULL;
	new->next = NULL;
	return (new);
}
