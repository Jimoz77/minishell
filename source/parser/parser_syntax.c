/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:51:30 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 18:06:20 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

static int	check_paren_content(t_token *start, t_token *end)
{
	t_token	*current;
	int		has_content;

	if (!start || !end || start == end)
		return (0);
	current = start;
	has_content = 0;
	while (current && current != end)
	{
		if (current->type == TOKEN_WORD || is_operator(current->type))
			has_content = 1;
		current = current->next;
	}
	return (has_content);
}

int	check_paren_balance(t_token *tokens)
{
	int		depth;
	t_token	*current;
	t_token	*paren_start;

	depth = 0;
	current = tokens;
	paren_start = NULL;
	while (current)
	{
		if (current->type == TOKEN_LPAREN)
		{
			if (depth == 0)
				paren_start = current;
			depth++;
		}
		else if (current->type == TOKEN_RPAREN)
		{
			depth--;
			if (depth < 0)
			{
				ft_printf("minishell: syntax error: unexpected token ')'\n");
				return (0);
			}
			if (depth == 0 && paren_start)
			{
				if (!check_paren_content(paren_start->next, current))
				{
					ft_printf("minishell: syntax error: empty parentheses\n");
					return (0);
				}
				paren_start = NULL;
			}
		}
		current = current->next;
	}
	if (depth > 0)
	{
		ft_printf("minishell: syntax error: expected ')'\n");
		return (0);
	}
	return (1);
}

int	is_valid_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_paren_balance(tokens))
		return (0);
	if (!check_start_operator(tokens)
		|| !check_redirections(tokens)
		|| !check_end_operator(tokens) 
		|| !check_consecutive_operators(tokens)
		|| !check_parentheses_usage(tokens))
		return (0);
	return (1);
}
