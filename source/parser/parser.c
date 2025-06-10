/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/09 23:21:42 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Renvoie la priorité d'un type de token (plus petit = plus prioritaire)
static int	get_priority(t_token_type type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (1);
	if (type == TOKEN_PIPE)
		return (2);
	return (100);
}

int	find_lowest_priority(t_token *tokens)
{
	int		pos;
	int		i;
	int		lowest;
	t_token	*tmp;
	int		depth;
	int		priority;

	pos = -1;
	i = 0;
	lowest = 100;
	depth = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_LPAREN)
			depth++;
		else if (tmp->type == TOKEN_RPAREN)
			depth--;
		if (depth == 0 && is_operator(tmp->type) && !is_redirection(tmp->type))
		{
			priority = get_priority(tmp->type);
			if (priority < lowest)
			{
				lowest = priority;
				pos = i;
			}
		}
		tmp = tmp->next;
		i++;
	}
	return (pos);
}

// Construit l'arbre de syntaxe (AST) à partir des tokens
t_node	*parse_ast(t_token *tokens)
{
	t_token	*op;
	int		i;
	t_node	*node;
	
	t_token	*tokens_copy;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	i = find_lowest_priority(tokens);
	if (i == -1)
	{
		// Faire une copie de la liste pour parse_command_with_redirections
		tokens_copy = tokens;
		node = parse_command_with_redirections(&tokens_copy);
		if (node)
			return (node);
	}
	op = get_token_at(tokens, i);
	if (!op)
	{
		// Faire une copie de la liste pour parse_command_with_redirections
		tokens_copy = tokens;
		node = parse_command_with_redirections(&tokens_copy);
		if (node)
			return (node);
	}
	return (create_op_node(tokens, op));
}
