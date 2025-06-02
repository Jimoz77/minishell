/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 16:29:11 by lsadikaj         ###   ########.fr       */
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

static int	update_lowest_priority(t_token *tmp, int i, int *pos)
{
	int	priority;

	priority = get_priority(tmp->type);
	if (priority < 100 || (priority == 100 && priority == 1))
	{
		*pos = i;
		return (priority);
	}
	return (100);
}

// Renvoie la position du token ayant la plus faible priorité
int	find_lowest_priority(t_token *tokens)
{
	int		pos;
	int		i;
	int		lowest;
	t_token	*tmp;
	int		depth;

	
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
			lowest = update_lowest_priority(tmp, i, &pos);
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

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	i = find_lowest_priority(tokens);
	if (i == -1)
		return (parse_command_with_redirections(&tokens));
	op = get_token_at(tokens, i);
	if (!op)
		return (parse_command_with_redirections(&tokens));
	return (create_op_node(tokens, op));
}
