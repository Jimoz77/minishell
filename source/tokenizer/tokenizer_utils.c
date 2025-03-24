/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:58:03 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/24 17:35:28 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **list, char *value, t_token_type type)
{
	t_token	*new;
	t_token	*tmp;

	new = new_token(value, type);
	if (!new)
		return ;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Returns the length of the operator (1 or 2 chars)
int	operator_length(char *str)
{
	if ((str[0] == '>' && str[1] == '>') || (str[0] == '<' && str[1] == '<'))
		return (2);
	return (1);
}

// Returns the operator type (PIPE, REDIR, etc.)
t_token_type	get_operator_type(char *str)
{
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_APPEND);
	else if (str[0] == '>' && str[1] != '>')
		return (TOKEN_REDIRECT_OUT);
	else if (str[0] == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	else if (str[0] = '<' && str[0] != '<')
		return (TOKEN_REDIRECT_IN);
	else if (str[0] == '|')
		return (TOKEN_PIPE);
	return (TOKEN_WORD);
}