/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
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
		else if (is_operator_str(&input[i]))
			i += handle_operator(&tokens, &input[i]);
		else if (input[i] == '"' || input[i] == '\'')
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
			ret = handle_word(&tokens, &input[i]);
			if (ret <= 0)
				return NULL;
			i += ret;
		}
	}
	return (tokens);
}