/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_handle.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:21:49 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/24 19:27:36 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Handles operator token (|, >, >>, <, <<) and adds it to the token list
int	handle_operator(t_token **tokens, char *input)
{
	int				len;
	char			*op;
	t_token_type	type;

	len = operator_length(input);
	op = ft_substr(input, 0, len);
	type = get_operator_type(input);
	add_token(tokens, op, type);
	free(op);
	return (len);
}

// Handles a plain word and adds it to the token list
int	handle_word(t_token **tokens, char *input)
{
	int		len;
	char	*word;

	len = 0;
	while (input[len])
	{
		if (input[len] == ' ' || input[len] == '\t')
			break ;
		if (input[len] == '|' || input[len] == '<' || input[len] == '>')
			break ;
		if (input[len] == '\'' || input[len] == '"')
			break ;
		len++;
	}
	word = ft_substr(input, 0, len);
	add_token(tokens. word, TOKEN_WORD);
	free(word);
	return (len);
}

// Handles a quoted string and adds it to the token list
int	handle_quotes(t_token **tokens, char *input)
{
	char	quote;
	int		len;
	char	*word;

	quote = input[0];
	len = 1;
	while (input[len] && input[len] != quote)
		len++;
	if (input[len] == quote)
	{
		word = ft_substr(input + 1, 0, len - 1);
		add_token(tokens, word, TOKEN_WORD);
		free(word);
		return (len + 1);
	}
	else
	{
		ft_printf("minishell: syntax error: unclosed quote\n");
		return (-1);
	}
}
