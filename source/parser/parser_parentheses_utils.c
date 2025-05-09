/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parentheses_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:04:20 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 11:00:59 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Traite une expression entre parenthèses
t_node	*parse_parenthesized_expr(t_token *tokens)
{
	t_token	*closing;
	t_token	*expr_tokens;
	t_node	*node;
	t_node	*content;

	if (!tokens || tokens->type != TOKEN_LPAREN)
		return (NULL);
	if (handle_arithmetic_expr(tokens))
		return (NULL);
	closing = find_matching_paren(tokens);
	if (!closing)
		return (NULL);
	expr_tokens = clone_tokens(tokens->next, closing);
	if (!expr_tokens)
		return (NULL);
	content = parse_ast(expr_tokens);
	free_tokens(expr_tokens);
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_PAREN;
	node->cmd = NULL;
	node->left = content;
	node->right = NULL;
	return (check_paren_operators(node, closing));
}

// Traite les expressions entre parenthèses avec opérateurs
t_node	*handle_paren_and_op(t_token *tokens)
{
	t_token	*next_op;
	int		i;
	t_token	*op;

	next_op = tokens;
	while (next_op && next_op->type != TOKEN_RPAREN)
		next_op = next_op->next;
	if (next_op && next_op->next)
	{
		i = find_lowest_priority(tokens);
		if (i != -1)
		{
			op = get_token_at(tokens, i);
			return (create_op_node(tokens, op));
		}
	}
	return (parse_parenthesized_expr(tokens));
}
