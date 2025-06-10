/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax_separate.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:48:03 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 16:32:56 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Traite une parenthèse fermante
static int	handle_close_paren(int *depth, t_token **paren_start,
								t_token *current)
{
	*depth = *depth - 1;
	if (*depth < 0)
	{
		ft_printf("minishell: syntax error: unexpected token ')'\n");
		return (0);
	}
	if (*depth == 0 && *paren_start)
	{
		if (!check_paren_content((*paren_start)->next, current))
		{
			ft_printf("minishell: syntax error: empty parentheses\n");
			return (0);
		}
		*paren_start = NULL;
	}
	return (1);
}

// Parcourt les tokens et traite les parenthèses
static int	process_tokens(t_token *tokens, int *depth, t_token **paren_start)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_LPAREN)
		{
			if (*depth == 0)
				*paren_start = current;
			(*depth)++;
		}
		else if (current->type == TOKEN_RPAREN)
		{
			if (!handle_close_paren(depth, paren_start, current))
				return (0);
		}
		current = current->next;
	}
	return (1);
}

int	check_paren_balance(t_token *tokens)
{
	int		depth;
	t_token	*paren_start;

	depth = 0;
	paren_start = NULL;
	if (!process_tokens(tokens, &depth, &paren_start))
		return (0);
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
