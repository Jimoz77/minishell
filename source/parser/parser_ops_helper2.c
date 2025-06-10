/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops_helper2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:37:28 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 16:53:01 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*split_at_pos(t_token *tokens, int pos)
{
	t_token	*prev;
	t_token	*op;
	t_token	*next_token;
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
	{
		next_token = op;
		prev->next = NULL;
		return (next_token);
	}
	return (NULL);
}

// Nettoie les ressources en cas d'erreur
static void	cleanup_op_node_error(t_node *node, t_token *left, t_token *right)
{
	if (left)
		free_tokens(left);
	if (right)
		free_tokens(right);
	free_ast(node);
}

// Finalise la création du noeud opérateur
static t_node	*finalize_op_node(t_node *node, t_token *left, t_token *right)
{
	if (!node->left || !node->right)
	{
		cleanup_op_node_error(node, left, right);
		return (NULL);
	}
	free_tokens(left);
	free_tokens(right);
	return (node);
}

// Trouve la parenthèse fermante correspondante
t_token	*find_matching_paren_public(t_token *start)
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

t_node	*create_op_node(t_token *tokens, t_token *op)
{
	t_node	*node;
	t_token	*left_tokens;
	t_token	*right_tokens;
	t_token	*op_token;
	int		op_pos;

	(void)op;
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	op_pos = find_lowest_priority(tokens);
	op_token = get_token_at(tokens, op_pos);
	if (!op_token)
	{
		free(node);
		return (NULL);
	}
	node->type = token_to_node_type(op_token->type);
	left_tokens = duplicate_tokens_until_pos(tokens, op_pos);
	right_tokens = split_at_pos(tokens, op_pos);
	if (right_tokens)
		right_tokens = delete_token(right_tokens, right_tokens);
	node->left = parse_ast(left_tokens);
	node->right = parse_ast(right_tokens);
	return (finalize_op_node(node, left_tokens, right_tokens));
}
