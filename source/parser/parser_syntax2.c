/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 16:00:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/29 16:37:24 by lsadikaj         ###   ########.fr       */
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
	while (tokens)
	{
		if (is_redirection(tokens->type))
		{
			// Vérifier si c'est le dernier token (rien après la redirection)
			if (!tokens->next)
			{
				ft_printf("minishell: syntax error near unexpected token `newline'\n");
				return (0);
			}
			// Vérifier si le token suivant est un opérateur ou une parenthèse fermante
			else if (is_operator(tokens->next->type) \
|| tokens->next->type == TOKEN_RPAREN)
			{
				ft_printf("minishell: syntax error near unexpected token '%s'\n",
					tokens->next->value);
				return (0);
			}
		}
		tokens = tokens->next;
	}
	return (1);
}

// Vérifie que la commande ne se termine pas par un opérateur
int	check_end_operator(t_token *tokens)
{
	t_token	*last;

	// Trouver le dernier token
	last = tokens;
	while (last && last->next)
		last = last->next;
	
	// Vérifier si le dernier token est un opérateur
	if (last && is_operator(last->type))
	{
		// Si c'est un pipe, utiliser un message similaire à bash
		if (last->type == TOKEN_PIPE)
			ft_printf("minishell: syntax error near unexpected token '|'\n");
		// De même pour les autres opérateurs
		else if (last->type == TOKEN_REDIRECT_IN)
			ft_printf("minishell: syntax error near unexpected token '<'\n");
		else if (last->type == TOKEN_REDIRECT_OUT)
			ft_printf("minishell: syntax error near unexpected token '>'\n");
		else if (last->type == TOKEN_APPEND)
			ft_printf("minishell: syntax error near unexpected token '>>'\n");
		else if (last->type == TOKEN_HEREDOC)
			ft_printf("minishell: syntax error near unexpected token '<<'\n");
		else if (last->type == TOKEN_AND)
			ft_printf("minishell: syntax error near unexpected token '&&'\n");
		else if (last->type == TOKEN_OR)
			ft_printf("minishell: syntax error near unexpected token '||'\n");
		else
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
	t_token	*current;
	
	current = tokens;
	while (current)
	{	
		// Vérifier les parenthèses vides
		if (current->type == TOKEN_LPAREN && current->next && current->next->type == TOKEN_RPAREN)
		{
			ft_printf("minishell: syntax error: empty parentheses\n");
			return (0);
		}
		
		// Vérifier qu'il y a quelque chose après '(' (sauf autre parenthèse)
		if (current->type == TOKEN_LPAREN && !current->next)
		{
			ft_printf("minishell: syntax error: unexpected end of file\n");
			return (0);
		}
		
		// Vérifier les parenthèses adjacentes sans opérateur (invalid: )(  )
		if (current->type == TOKEN_RPAREN && current->next && current->next->type == TOKEN_LPAREN)
		{
			ft_printf("minishell: syntax error near unexpected token `('\n");
			return (0);
		}
		current = current->next;
	}
	return (1);
}
