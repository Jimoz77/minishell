/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops_helper4.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 15:33:04 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/08 15:35:11 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ajoute un token à la liste pour duplicate_tokens_until_pos
static void	add_token_until_pos(t_token **new_tokens, t_token **prev,
							t_token *new_current)
{
	if (!*new_tokens)
		*new_tokens = new_current;
	else
		(*prev)->next = new_current;
	*prev = new_current;
}

// Traite un token dans la boucle de duplication
static int	process_one_token(t_token **new_tokens, t_token **prev,
							t_token *current)
{
	t_token	*new_current;

	new_current = malloc(sizeof(t_token));
	if (!new_current)
	{
		if (*new_tokens)
			free_tokens(*new_tokens);
		return (0);
	}
	if (current->value)
		new_current->value = ft_strdup(current->value);
	else
		new_current->value = NULL;
	new_current->type = current->type;
	new_current->parts = duplicate_word_parts(current->parts);
	new_current->next = NULL;
	add_token_until_pos(new_tokens, prev, new_current);
	return (1);
}

t_token	*duplicate_tokens_until_pos(t_token *tokens, int pos)
{
	t_token	*new_tokens;
	t_token	*current;
	t_token	*prev;
	int		i;

	if (!tokens || pos <= 0)
		return (NULL);
	new_tokens = NULL;
	prev = NULL;
	current = tokens;
	i = 0;
	while (current && i < pos)
	{
		if (!process_one_token(&new_tokens, &prev, current))
			return (NULL);
		current = current->next;
		i++;
	}
	return (new_tokens);
}
