/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:37:41 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/26 15:32:29 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si un caractère est un espace ou une tabulation
int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

// Découpe la ligne d’entrée en une liste chaînée de tokens
t_token	*tokenize(char *input)
{
	t_token			*tokens;
	int				i;
	int				ret;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (is_operator_str(&input[i]))
			i += handle_operator(&tokens, &input[i]);
		else if (input[i] == '"' || input[i] == '\'')
		{
			ret = handle_quotes(&tokens, &input[i]);
			if (ret == -1)
				return (NULL);
			i += ret;
		}
		else
		{
			ret = handle_word(&tokens, &input[i]);
			if (ret <= 0)
				return NULL;
			i += ret;
		}
	}
	return (tokens);
}
