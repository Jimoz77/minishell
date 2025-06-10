/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:26:53 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:36:45 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Ajoute l'entrée si elle correspond au motif et alloue la chaîne à matches[i]
int	add_if_match(char **matches, int i, const char *pattern,
				struct dirent *entry)
{
	if (entry->d_name[0] != '.' || pattern[0] == '.')
	{
		if (match_pattern(pattern, entry->d_name))
		{
			matches[i] = ft_strdup(entry->d_name);
			if (!matches[i])
				return (-1);
			return (1);
		}
	}
	return (0);
}

// Libère les chaînes partiellement allouées en cas d'erreur + ferme le dossier
char	**free_matches_partial(char **matches, int i, DIR *dir)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(matches[j]);
		j++;
	}
	free(matches);
	closedir(dir);
	return (NULL);
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
