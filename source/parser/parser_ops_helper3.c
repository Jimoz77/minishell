/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops_helper3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:37:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 16:56:00 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_tokens_until(t_token *start, t_token *end)
{
	t_token	*current;
	int		count;

	if (!start || !end)
		return (0);
	current = start;
	count = 0;
	while (current && current != end)
	{
		count++;
		current = current->next;
	}
	return (count);
}

// Ajoute un token à la liste pour duplicate_tokens_until_pos
void	add_token_until_pos(t_token **new_tokens, t_token **prev,
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

t_node	*handle_paren_and_op(t_token *tokens)
{
	t_token	*end_paren;
	t_token	*inner_start;
	t_token	*inner_tokens;
	t_node	*inner_node;
	int		inner_count;

	if (!tokens || tokens->type != TOKEN_LPAREN)
		return (NULL);
	end_paren = find_matching_paren_public(tokens);
	if (!end_paren)
		return (NULL);
	inner_start = tokens->next;
	if (inner_start == end_paren)
		return (NULL);
	inner_count = count_tokens_until(inner_start, end_paren);
	inner_tokens = duplicate_tokens_until_pos(inner_start, inner_count);
	if (!inner_tokens)
		return (NULL);
	inner_node = parse_ast(inner_tokens);
	free_tokens(inner_tokens);
	if (!inner_node)
		return (NULL);
	if (end_paren->next && is_operator(end_paren->next->type))
		return (handle_op_after_paren(inner_node, tokens, end_paren->next));
	return (inner_node);
}
