/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 13:37:41 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/24 17:35:11 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_space(char c)
{
	return (c == ' ' || c == '\t');
}

static int	word_len(char *str)
{
	int	len;

	len = 0;
	while (str[len] && !is_space(str[len]))
		len++;
	return (len);
}

t_token	*tokenize(char *input)
{
	t_token			*tokens = NULL;
	int				i;
	int				len;
	char			*word;
	t_token_type	type;

	i = 0;
	while (input[i])
	{
		if (is_space(input[i]))
			i++;
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		{
			len = operator_length(&input[i]);
			word = ft_substr(input, i, len);
			type = get_operator_type(&input[i]);
			add_token(&tokens. word, type);
			free(word);
			i += len;
		}
		else
		{
			len = word_len(&input[i]);
			word = ft_substr(input, i, len);
			add_token(&tokens, word, TOKEN_WORD);
			free(word);
			i += len;
		}
	}
	return (tokens);
}