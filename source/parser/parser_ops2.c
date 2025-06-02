/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 17:03:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*find_token_before(t_token *start, t_token *target)
{
	t_token	*current;
	t_token	*prev;

	if (!start || !target || start == target)
		return (NULL);
	current = start;
	prev = NULL;
	while (current && current != target)
	{
		prev = current;
		current = current->next;
	}
	return (prev);
}

t_node	*handle_op_after_paren(t_node *inner_node, t_token *op_token)
{
	t_node	*op_node;

	if (!inner_node || !op_token)
		return (inner_node);
	op_node = ft_calloc(1, sizeof(t_node));
	if (!op_node)
	{
		free_ast(inner_node);
		return (NULL);
	}
	op_node->type = token_to_node_type(op_token->type);
	op_node->left = inner_node;
	if (op_token->next)
		op_node->right = parse_ast(op_token->next);
	else
		op_node->right = NULL;
	if (!op_node->right && op_node->type != NODE_CMD)
	{
		free_ast(op_node);
		return (NULL);
	}
	return (op_node);
}
