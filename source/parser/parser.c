/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/08 14:12:24 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_priority(t_token_type type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (2);
	if (type == TOKEN_PIPE)
		return (1);
	return (100);
}

static int	process_token(t_token *token, int *depth, int *pos, int *lowest)
{
	int	priority;
	int	i;

	i = *pos;
	if (token->type == TOKEN_LPAREN)
		(*depth)++;
	else if (token->type == TOKEN_RPAREN)
		(*depth)--;
	if (*depth == 0 && is_operator(token->type) && !is_redirection(token->type))
	{
		priority = get_priority(token->type);
		if (priority >= *lowest)
		{
			*lowest = priority;
			return (i);
		}
	}
	return (-1);
}

int	find_lowest_priority(t_token *tokens)
{
	t_token	*tmp;
	int		depth;
	int		lowest;
	int		pos;
	int		i;

	tmp = tokens;
	depth = 0;
	lowest = 0;
	pos = -1;
	i = 0;
	while (tmp)
	{
		if (process_token(tmp, &depth, &i, &lowest) != -1)
			pos = i;
		tmp = tmp->next;
		i++;
	}
	return (pos);
}

static t_node	*handle_no_operator(t_token *tokens)
{
	t_token	*tokens_copy;

	tokens_copy = tokens;
	return (parse_command_with_redirections(&tokens_copy));
}

t_node	*parse_ast(t_token *tokens)
{
	t_token	*op;
	int		i;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	i = find_lowest_priority(tokens);
	if (i == -1)
		return (handle_no_operator(tokens));
	op = get_token_at(tokens, i);
	if (!op)
		return (handle_no_operator(tokens));
	return (create_op_node(tokens, op));
}
