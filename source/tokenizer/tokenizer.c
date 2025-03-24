/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:37:41 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/24 19:22:17 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Check if a character is a space or tab
int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

// Calculates the length of a word (non-espace characters)
static int	word_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && !is_space(str[len]))
		len++;
	return (len);
}

// Tokenizes the input line into a linked list of tokens
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
		else if (is_operator(&input[i]))
			i += handle_operator(&tokens, &input[i]);
		else if (input[i] == '"' || input[i] == '\'')
		{
			ret = handle_quotes(&tokens, &input[i]);
			if (ret == -1)
				return (NULL);
			i += ret;
		}
		else
			i += handle_word(&tokens, &input[i]);
	}
	return (tokens);
}
