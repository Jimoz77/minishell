/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/08 13:42:22 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_token_at(t_token *tokens, int pos)
{
	int	i;

	i = 0;
	while (tokens && i < pos)
	{
		tokens = tokens->next;
		i++;
	}
	return (tokens);
}

static t_token	*split_at_pos(t_token *tokens, int pos)
{
	t_token	*prev;
	t_token	*op;
	int		i;

	if (pos == 0)
		return (tokens);
	prev = tokens;
	i = 0;
	while (i < pos - 1 && prev->next)
	{
		prev = prev->next;
		i++;
	}
	op = prev->next;
	if (op)
		prev->next = NULL;
	return (op);
}

t_node	*create_op_node(t_token *tokens, t_token *op)
{
	t_node	*node;
	t_token	*left_tokens;
	t_token	*right_tokens;

	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token_to_node_type(op->type);
	left_tokens = tokens;
	op = split_at_pos(tokens, find_lowest_priority(tokens));
	if (op && op->next)
		right_tokens = op->next;
	else
		right_tokens = NULL;
	node->left = parse_ast(left_tokens);
	node->right = parse_ast(right_tokens);
	if (!node->left || !node->right)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

static t_token	*find_matching_paren(t_token *start)
{
	t_token	*current;
	int		depth;

	current = start->next;
	depth = 1;
	while (current && depth > 0)
	{
		if (current->type == TOKEN_LPAREN)
			depth++;
		else if (current->type == TOKEN_RPAREN)
			depth--;
		if (depth == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_node	*handle_paren_and_op(t_token *tokens) // surement util !!!!!!!!!!!!
{
	t_token	*end_paren;
	t_token	*inner_start;
	t_token	*inner_end;
	t_node	*inner_node;

	if (!tokens || tokens->type != TOKEN_LPAREN)
		return (NULL);
	end_paren = find_matching_paren(tokens);
	if (!end_paren)
		return (NULL);
	inner_start = tokens->next;
	inner_end = end_paren;
	if (inner_start == inner_end)
		return (NULL);
	inner_end = find_token_before(inner_start, end_paren);
	if (inner_end)
		inner_end->next = NULL;
	inner_node = parse_ast(inner_start);
	if (!inner_node)
		return (NULL);
	if (end_paren->next && is_operator(end_paren->next->type))
		return (handle_op_after_paren(inner_node, inner_start, end_paren->next));
	return (inner_node);
}
