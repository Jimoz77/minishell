/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 17:17:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 17:59:21 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Vérifie si une chaîne contient un caractère wildcard (*) non protégé par 
// des guillemets. Retourne 1 si un wildcard est trouvé, 0 sinon.
int	contains_wildcard(const char *str)
{
	int		i;
	int		in_quotes;
	char	quote_type;

	if (!str)
		return (0);
	i = 0;
	in_quotes = 0;
	quote_type = 0;
	while (str[i])
	{
		if (!in_quotes && (str[i] == '\'' || str[i] == '"'))
		{
			in_quotes = 1;
			quote_type = str[i];
		}
		else if (in_quotes && str[i] == quote_type)
			in_quotes = 0;
		else if (!in_quotes && str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

// Remplace un token contenant un wildcard par plusieurs tokens représentant
// les fichiers correspondants. Retourne le premier token de la nouvelle liste.
t_token	*replace_with_matches(t_token *token, char **matches, int count)
{
	t_token	*first;
	t_token	*current;
	t_token	*next;
	int		i;

	if (count == 0)
		return (token);
	first = NULL;
	current = NULL;
	next = token->next;
	i = 0;
	while (i < count)
	{
		current = create_match_token(matches[i]);
		if (!current)
		{
			i++;
			continue;
		}
		if (!first)
			first = current;
		else
			add_token_to_list(&first, current);
		i++;
	}
	if (current)
		current->next = next;
	free_token_content(token);
	free(token);
	return (first ? first : next);
}

// Parcourt tous les tokens et remplace ceux contenant des wildcards par
// les fichiers correspondants dans le répertoire courant.
void	expand_wildcards(t_token *tokens)
{
	t_token	*current;
	t_token	*prev;
	char	**matches;
	int		count;

	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD && contains_wildcard(current->value))
		{
			count = count_matching_files(current->value);
			if (count > 0)
				process_wildcard_matches(current, prev, &matches, count);
		}
		prev = current;
		if (current)
			current = current->next;
	}
}
