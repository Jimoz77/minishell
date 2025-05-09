/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 14:52:29 by lsadikaj         ###   ########.fr       */
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

// Vérifie si un token opérateur a une priorité plus faible
static int	check_token_priority(t_token *token, int depth, int *lowest)
{
	int	priority;

	if (depth == 0 && is_operator(token->type))
	{
		priority = get_priority(token->type);
		if (priority <= *lowest)
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
		else if (check_token_priority(tmp, depth, &lowest))
			pos = i;
		tmp = tmp->next;
		i++;
	}
	return (pos);
}

// Vérifie et traite une redirection suivie d'un mot
static t_node	*handle_redir_and_word(t_token *tokens)
{
	t_token	*tmp;

	if (!tokens || !tokens->next || !tokens->next->next)
		return (NULL);
	tmp = tokens->next->next;
	while (tmp && tmp->type == TOKEN_WORD)
		tmp = tmp->next;
	if (tmp && is_operator(tmp->type))
		return (create_op_node(tokens, tokens));
	return (NULL);
}

// Construit l'arbre de syntaxe (AST) à partir des tokens
t_node	*parse_ast(t_token *tokens)
{
	t_token	*op;
	int		i;
	t_node	*node;

	if (!tokens)
		return (NULL);
	if (is_redirection(tokens->type) && tokens->next && tokens->next->next)
	{
		node = handle_redir_and_word(tokens);
		if (node)
			return (node);
	}
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	i = find_lowest_priority(tokens);
	if (i == -1 || (tokens->next && !tokens->next->next
			&& tokens->type == TOKEN_WORD && tokens->next->type == TOKEN_WORD))
		return (create_cmd_node(tokens));
	op = get_token_at(tokens, i);
	if (!op)
		return (create_cmd_node(tokens));
	return (create_op_node(tokens, op));
}
