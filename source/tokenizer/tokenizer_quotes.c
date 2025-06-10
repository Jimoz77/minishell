/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:41:03 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:54:52 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Parse une portion non citée d'un mot et l'ajoute à la liste des parties
static int	parse_unquoted_part(char *input, int *i, t_word_part **parts)
{
	int		start;
	char	*content;

	start = *i;
	while (input[*i] && input[*i] != '"' && input[*i] != '\''
		&& !is_space(input[*i]) && !is_operator_str(&input[*i])
		&& input[*i] != '(' && input[*i] != ')')
		(*i)++;
	if (start == *i)
		return (0);
	content = ft_substr(input, start, *i - start);
	if (!content)
		return (-1);
	add_word_part(parts, content, QUOTE_NONE);
	free(content);
	return (0);
}

// Parse un mot complexe composé de portions citées et non citées
static int	parse_complex_word(char *input, int *i, t_word_part **parts)
{
	int	start;

	start = *i;
	while (input[*i] && !is_space(input[*i]) && !is_operator_str(&input[*i])
		&& input[*i] != '(' && input[*i] != ')')
	{
		if (input[*i] == '"')
		{
			if (parse_quoted_part(input, i, parts, QUOTE_DOUBLE) == -1)
				return (-1);
		}
		else if (input[*i] == '\'')
		{
			if (parse_quoted_part(input, i, parts, QUOTE_SINGLE) == -1)
				return (-1);
		}
		else if (parse_unquoted_part(input, i, parts) == -1)
			return (-1);
	}
	return (*i - start);
}

// Libère les parties d'un mot complexe en cas d'erreur
int	clean_complex_word(t_word_part *parts, char *value, int ret)
{
	if (parts)
		free_word_parts(parts);
	if (value)
		free(value);
	return (ret);
}

// Ajoute le nouveau token à la liste des tokens
void	add_new_token(t_token **tokens, t_token *new_token)
{
	t_token	*tmp;

	if (!*tokens)
		*tokens = new_token;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

// Analyse un mot contenant des guillemets et l'ajoute à la liste des tokens
int	handle_complex_word(t_token **tokens, char *input)
{
	t_token		*new_token;
	t_word_part	*parts;
	int			i;
	int			len;

	parts = NULL;
	i = 0;
	len = parse_complex_word(input, &i, &parts);
	if (len <= 0)
		return (clean_complex_word(parts, NULL, len));
	if (len == 2)
		new_token = create_empty_token(parts);
	else
		new_token = create_complex_token(input, len, parts);
	if (!new_token)
		return (-1);
	add_new_token(tokens, new_token);
	return (len);
}
