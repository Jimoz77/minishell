/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:37:41 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/03 13:09:33 by jimpa            ###   ########.fr       */
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
		*i += handle_operator(tokens, &input[*i]);
	else if (input[*i] == '"' || input[*i] == '\'')
	{
		ret = handle_quotes(tokens, &input[*i]);
		if (ret == -1)
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

// Découpe la ligne d’entrée en une liste chaînée de tokens

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;

	i = 0;
	while (input[i])
	{
		if (handle_token(&tokens, input, &i) == -1)
			return (NULL);
	}
	return (tokens);
}