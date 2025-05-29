/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parentheses_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 15:04:20 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/29 17:22:31 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Traite une expression entre parenthèses
t_node	*parse_parenthesized_expr(t_token *tokens)
{
	t_token	*closing;
	t_token	*expr_tokens;
	t_node	*paren_node;
	t_node	*content;

	if (!tokens || tokens->type != TOKEN_LPAREN)
		return (NULL);
	closing = find_matching_paren(tokens);
	if (!closing)
	{
		ft_printf("minishell: syntax error: unmatched '('\n");
		return (NULL);
	}
	expr_tokens = clone_tokens(tokens->next, closing);
	if (!expr_tokens)
	{
		ft_printf("minishell: syntax error: empty parentheses\n");
		return (NULL);
	}
	content = parse_ast(expr_tokens);
	free_tokens(expr_tokens);
	if (!content)
		return (NULL);
	paren_node = malloc(sizeof(t_node));
	if (!paren_node)
	{
		free_ast(content);
		return (NULL);
	}
	paren_node->type = NODE_PAREN;
	paren_node->cmd = NULL;
	paren_node->left = content;
	paren_node->right = NULL;
	return (check_paren_operators(paren_node, closing));
}

// Traite les expressions entre parenthèses avec opérateurs
t_node	*handle_paren_and_op(t_token *tokens)
{
	t_token	*current;
	int		paren_depth;
	int		op_position;
	int		position;
	t_token	*first_closing;
	t_token	*op;

	current = tokens;
	paren_depth = 0;
	op_position = -1;
	position = 0;
	first_closing = NULL;
	while (current)
	{
		if (current->type == TOKEN_LPAREN)
			paren_depth++;
		else if (current->type == TOKEN_RPAREN)
		{
			paren_depth--;
			if (paren_depth == 0 && !first_closing)
			{
				first_closing = current;
				break ;
			}
		}
		current = current->next;
		position++;
	}
	// Deuxième passe : chercher des opérateurs après la première fermeture complète
	if (first_closing && first_closing->next)
	{
		current = first_closing->next;
		position++;
		while (current)
		{
			if (is_operator(current->type))
			{
				op_position = position;
				break ;
			}
			current = current->next;
			position++;
		}
	}
	// S'il y a un opérateur après la première parenthèse complète, traiter comme opérateur
	if (op_position != -1)
	{
		op = get_token_at(tokens, op_position);
		return (create_op_node(tokens, op));
	}
	return (parse_parenthesized_expr(tokens));
}
