/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:00:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/23 16:15:47 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si les opérateurs sont bien positionnés au début
int	check_start_operator(t_token *tokens)
{
	if (tokens && is_operator(tokens->type))
	{
		if (tokens->type != TOKEN_HEREDOC
			&& tokens->type != TOKEN_REDIRECT_IN
			&& tokens->type != TOKEN_REDIRECT_OUT
			&& tokens->type != TOKEN_APPEND)
		{
			ft_printf("minishell: syntax error near unexpected token '%s'\n",
				tokens->value);
			return (0);
		}
	}
	return (1);
}

// Vérifie les redirections sans cible
int	check_redirections(t_token *tokens)
{
	while (tokens && tokens->next)
	{
		if (is_redirection(tokens->type)
			&& (is_operator(tokens->next->type)
				|| tokens->next->type == TOKEN_RPAREN))
		{
			ft_printf("minishell: syntax error near unexpected token '%s'\n",
				tokens->next->value);
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

// Vérifie que la commande ne se termine pas par un opérateur
int	check_end_operator(t_token *tokens)
{
	while (tokens && tokens->next)
		tokens = tokens->next;
	if (tokens && is_operator(tokens->type))
	{
		ft_printf("minishell: syntax error: unexpected end of file\n");
		return (0);
	}
	return (1);
}

// Vérifie si deux opérateurs logiques se suivent
int	check_consecutive_operators(t_token *tokens)
{
	while (tokens && tokens->next)
	{
		if ((tokens->type == TOKEN_AND || tokens->type == TOKEN_OR
				|| tokens->type == TOKEN_PIPE)
			&& (tokens->next->type == TOKEN_AND
				|| tokens->next->type == TOKEN_OR
				|| tokens->next->type == TOKEN_PIPE))
		{
			ft_printf("minishell: syntax error near unexpected token '%s'\n",
				tokens->next->value);
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}

// Vérifie l'utilisation des parenthèses
int	check_parentheses_usage(t_token *tokens)
{
	while (tokens && tokens->next)
	{
		if (tokens->type == TOKEN_RPAREN && tokens->next->type == TOKEN_LPAREN)
		{
			ft_printf("minishell: syntax error near unexpected token '('\n");
			return (0);
		}
		if (tokens->type == TOKEN_LPAREN && tokens->next->type == TOKEN_RPAREN)
		{
			ft_printf("minishell: syntax error: empty parentheses\n");
			return (0);
		}
		tokens = tokens->next;
	}
	return (1);
}
