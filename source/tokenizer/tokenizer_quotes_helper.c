/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:50:34 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 14:55:31 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Crée un token à partir d'un mot complexe, avec ses parties
t_token	*create_complex_token(char *input, int len, t_word_part *parts)
{
	t_token		*new_token;
	char		*value;

	value = ft_substr(input, 0, len);
	if (!value)
	{
		free_word_parts(parts);
		return (NULL);
	}
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(value);
		free_word_parts(parts);
		return (NULL);
	}
	new_token->value = value;
	new_token->type = TOKEN_WORD;
	new_token->parts = parts;
	new_token->next = NULL;
	return (new_token);
}

// Crée un token avec une valeur vide quand nécessaire
t_token	*create_empty_token(t_word_part *parts)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free_word_parts(parts);
		return (NULL);
	}
	new_token->value = ft_strdup("");
	if (!new_token->value)
	{
		free(new_token);
		free_word_parts(parts);
		return (NULL);
	}
	new_token->type = TOKEN_WORD;
	new_token->parts = parts;
	new_token->next = NULL;
	return (new_token);
}
