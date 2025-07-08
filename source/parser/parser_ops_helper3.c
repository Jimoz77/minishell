/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops_helper3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:37:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/08 16:06:04 by lsadikaj         ###   ########.fr       */
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

t_node	*create_paren_node(t_node *inner_node)
{
	t_node	*paren_node;

	paren_node = ft_calloc(1, sizeof(t_node));
	if (!paren_node)
	{
		free_ast(inner_node);
		return (NULL);
	}
	paren_node->type = NODE_PAREN;
	paren_node->left = inner_node;
	paren_node->right = NULL;
	paren_node->cmd = NULL;
	paren_node->redirections = NULL;
	paren_node->heredocs = NULL;
	return (paren_node);
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
	return (create_paren_node(inner_node));
}
