/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:37:41 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/09 15:17:46 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si un caractère est un espace ou une tabulation
int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

// Gère les tokens autres que les espaces et parenthèses
static int	handle_token2(t_token **tokens, char *input, int *i)
{
	int	ret;

	if (is_operator_str(&input[*i]))
	{
		ret = handle_operator(tokens, &input[*i]);
		if (ret <= 0)
			return (-1);
		*i += ret;
	}
	else if (input[*i] == '"' || input[*i] == '\'')
	{
		ret = handle_complex_word(tokens, &input[*i]);
		if (ret <= 0)
			return (-1);
		*i += ret;
	}
	else
	{
		ret = handle_word(tokens, &input[*i]);
		if (ret <= 0)
			return (-1);
		*i += ret;
	}
	return (0);
}

// Gère un token unique à la position actuelle
static int	handle_token(t_token **tokens, char *input, int *i)
{
	int	ret;

	if (is_space(input[*i]))
		(*i)++;
	else if (input[*i] == '(' || input[*i] == ')')
	{
		ret = handle_parenthesis(tokens, &input[*i]);
		if (ret <= 0)
			return (-1);
		*i += ret;
	}
	else
		return (handle_token2(tokens, input, i));
	return (0);
}

// Parcours l'input et la transforme en liste de token
t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (handle_token(&tokens, input, &i) == -1)
		{
			free_tokens(tokens);
			return (NULL);
		}
	}
	return (tokens);
}
