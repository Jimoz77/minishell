/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:18:10 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 17:59:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Compte le nombre de fichiers dans le répertoire courant qui correspondent
// au motif de wildcard. Ignore les fichiers cachés sauf si le motif commence par '.'.
int	count_matching_files(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match_pattern(pattern, entry->d_name))
			count++;
	}
	closedir(dir);
	return (count);
}

// Récupère la liste des fichiers correspondant au motif dans un tableau.
// Alloue un tableau de 'count' éléments + 1 (pour le NULL terminal).
char	**get_matching_files(const char *pattern, int count)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**matches;
	int				i;

	matches = malloc(sizeof(char *) * (count + 1));
	if (!matches)
		return (NULL);
	dir = opendir(".");
	if (!dir)
	{
		free(matches);
		return (NULL);
	}
	i = 0;
	while ((entry = readdir(dir)) != NULL && i < count)
	{
		if (entry->d_name[0] == '.' && pattern[0] != '.')
			continue;
		if (match_pattern(pattern, entry->d_name))
			matches[i++] = ft_strdup(entry->d_name);
	}
	matches[i] = NULL;
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

// Ajoute un token à la fin d'une liste chaînée de tokens.
// Parcourt la liste jusqu'au dernier élément et y attache le nouveau token.
void	add_token_to_list(t_token **first, t_token *new)
{
	t_token	*tmp;

	tmp = *first;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
