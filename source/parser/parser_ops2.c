/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/08 16:06:41 by lsadikaj         ###   ########.fr       */
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

// Traite les redirections après parenthèses
static t_node	*process_redirections(t_node *current_node, t_token **current)
{
	while (*current && is_redirection((*current)->type))
	{
		if ((*current)->next && (*current)->next->value)
		{
			add_redirection(current_node, (*current)->type,
				(*current)->next->value);
			*current = (*current)->next;
			if (*current)
				*current = (*current)->next;
		}
		else
			break ;
	}
	return (current_node);
}

// Crée un noeud opérateur après parenthèses
static t_node	*create_operator_node(t_node *current_node, t_token *current)
{
	t_node	*op_node;

	op_node = ft_calloc(1, sizeof(t_node));
	if (!op_node)
	{
		free_ast(current_node);
		return (NULL);
	}
	op_node->type = token_to_node_type(current->type);
	op_node->left = create_paren_node(current_node);
	if (current->next)
		op_node->right = parse_ast(current->next);
	return (op_node);
}

t_node	*handle_op_after_paren(t_node *inner_node, t_token *inner_start,
								t_token *op_token)
{
	t_node	*current_node;
	t_token	*current_token;

	(void)inner_start;
	if (!inner_node || !op_token)
		return (inner_node);
	current_node = inner_node;
	current_token = op_token;
	current_node = process_redirections(current_node, &current_token);
	if (current_token && !is_redirection(current_token->type))
		return (create_operator_node(current_node, current_token));
	return (current_node);
}
