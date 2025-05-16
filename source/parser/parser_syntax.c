/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:51:30 by lsadikaj          #+#    #+#             */

/*   Updated: 2025/04/23 16:13:15 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si un token est un opérateur
int	is_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

// Vérifie si un token est une redirection
int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

// Vérifie l'équilibre des parenthèses
int	check_paren_balance(t_token *tokens)
{
	int	depth;

	depth = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_LPAREN)
			depth++;
		else if (tokens->type == TOKEN_RPAREN)
		{
			depth--;
			if (depth < 0)
				return (0);
		}
		tokens = tokens->next;
	}
	return (depth == 0);
}

// Vérifie que la syntaxe est valide
int	is_valid_syntax(t_token *tokens)
{
	if (!tokens)
		return (1);
	if (!check_paren_balance(tokens))
	{
		ft_printf("minishell: syntax error: unbalanced parentheses\n");
		return (0);
	}
	if (!check_start_operator(tokens) || !check_redirections(tokens)
		|| !check_end_operator(tokens) || !check_consecutive_operators(tokens)
		|| !check_parentheses_usage(tokens))
		return (0);
	return (1);
}
