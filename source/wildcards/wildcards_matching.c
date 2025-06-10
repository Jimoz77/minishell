/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_matching.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:19:57 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:22:04 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Vérifie si une chaîne correspond à un motif contenant des wildcards (*).
// Utilise une approche récursive pour tester toutes les possibilités.
int	match_pattern(const char *pattern, const char *string)
{
	if (*pattern == '\0')
		return (*string == '\0');
	if (*pattern == '*')
	{
		pattern++;
		while (*string)
		{
			if (match_pattern(pattern, string))
				return (1);
			string++;
		}
		return (match_pattern(pattern, string));
	}
	if (*pattern == *string)
		return (match_pattern(pattern + 1, string + 1));
	return (0);
}

// Traite les correspondances wildcards pour un token.
// Récupère les fichiers correspondants, les trie, et remplace le token.
void	process_wildcard_matches(t_token *current, t_token *prev,
								char ***matches_ptr, int count)
{
	char	**matches;
	t_token	*new_token;

	matches = get_matching_files(current->value, count);
	if (matches)
	{
		sort_strings(matches, count);
		new_token = replace_with_matches(current, matches, count);
		if (prev)
			prev->next = new_token;
		current = new_token;
		free_matches(matches, count);
	}
	*matches_ptr = matches;
}

// Libère la mémoire allouée pour un tableau de chaînes.
// Libère d'abord chaque chaîne, puis le tableau lui-même.
void	free_matches(char **matches, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(matches[i]);
		i++;
	}
	free(matches);
}

// Libère le contenu d'un token sans libérer le token lui-même.
// Gère les cas où la valeur ou les parties peuvent être NULL.
void	free_token_content(t_token *token)
{
	if (token->value)
		free(token->value);
	if (token->parts)
		free_word_parts(token->parts);
}
