/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:03:50 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/26 15:32:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Crée un nouveau token avec une valeur et un type
t_token *new_token(char *value, t_token_type type)
{
	t_token *token;

	if (!value)
		return (NULL);
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (free(token), NULL);
	token->type = type;
	token->next = NULL;
	return (token);
}

// Ajoute un token à la fin de la liste
void add_token(t_token **list, char *value, t_token_type type)
{
	if (!value || !*value)
		return ;
	t_token *new = new_token(value, type);
	if (!new)
		return ;
	if (!*list)
		*list = new;
	else
	{
		t_token *tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Vérifie si une chaîne commence par un opérateur
int	is_operator_str(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '|' || str[0] == '<' || str[0] == '>' || str[0] == '&')
		return (1);
	return (0);
}

// Retourne la longueur d'un opérateur (1 ou 2 caractères)
int	operator_length(char *str)
{
	if ((str[0] == '>' && str[1] == '>') ||
		(str[0] == '<' && str[1] == '<') ||
		(str[0] == '&' && str[1] == '&') ||
		(str[0] == '|' && str[1] == '|'))
		return (2);
	return (1);
}

// Retourne le type de token associé à un opérateur
t_token_type	get_operator_type(char *str)
{
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	else if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	else if (str[0] == '>' && str[1] == '>')
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