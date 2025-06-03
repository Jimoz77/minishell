/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 14:30:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 17:02:42 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_start_operator(t_token *tokens)
{
	if (tokens && is_operator(tokens->type))
	{
		if (!is_redirection(tokens->type))
		{
			ft_printf("minishell: syntax error near unexpected token `%s'\n",
				tokens->value);
			return (0);
		}
	}
	return (1);
}

int	check_end_operator(t_token *tokens)
{
	t_token	*last;

	if (!tokens)
		return (1);
	last = tokens;
	while (last->next)
		last = last->next;
	if (is_operator(last->type) && !is_redirection(last->type))
	{
		ft_printf("minishell: syntax error: unexpected end of input\n");
		return (0);
	}
	return (1);
}

int	check_consecutive_operators(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (is_operator(current->type) && !is_redirection(current->type)
			&& is_operator(current->next->type)
			&& !is_redirection(current->next->type))
		{
			ft_printf("minishell: syntax error near unexpected token `%s'\n",
				current->next->value);
			return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				ft_printf("minishell: syntax error near unexpected token\n");
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}

int	check_parentheses_usage(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_LPAREN && current->next
			&& current->next->type == TOKEN_RPAREN)
		{
			ft_printf("minishell: syntax error: empty parentheses\n");
			return (0);
		}
		if (current->type == TOKEN_RPAREN && current->next
			&& current->next->type == TOKEN_LPAREN)
		{
			ft_printf("minishell: syntax error near unexpected token `('\n");
			return (0);
		}
		current = current->next;
	}
	return (1);
}
