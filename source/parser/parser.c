/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/28 13:10:51 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Renvoie la priorité d'un type de token (plus petit = plus prioritaire)
static int	get_priority(t_token_type type)
{
	if (type == TOKEN_OR)
		return (1);
	if (type == TOKEN_AND)
		return (2);
	if (type == TOKEN_PIPE)
		return (3);
	if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (4);
	return (100);
}

// Vérifie si un token est un opérateur de priorité plus basse
static int	is_lower_priority(t_token *token, int depth, int *lowest)
{
	int	priority;

	if (depth == 0)
	{
		priority = get_priority(token->type);
		if (priority < *lowest)
		{
			*lowest = priority;
			return (1);
		}
	}
	return (0);
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
		else if (is_lower_priority(tmp, depth, &lowest))
			pos = i;
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

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	if (!tokens->next)
		return (create_cmd_node(tokens));
	i = find_lowest_priority(tokens);
	op = get_token_at(tokens, i);
	if (!op || i == -1)
		return (create_cmd_node(tokens));
	return (create_op_node(tokens, op));
}
