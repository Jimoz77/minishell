/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops_helper1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:37:10 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 16:42:55 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Copie la valeur d'un token
static int	copy_token_value(t_token *new_current, t_token *current)
{
	if (current->value)
	{
		new_current->value = ft_strdup(current->value);
		if (!new_current->value)
			return (0);
	}
	else
		new_current->value = NULL;
	return (1);
}

// Crée un nouveau token
static t_token	*create_new_token(t_token *current)
{
	t_token	*new_current;

	new_current = malloc(sizeof(t_token));
	if (!new_current)
		return (NULL);
	if (!copy_token_value(new_current, current))
	{
		free(new_current);
		return (NULL);
	}
	new_current->type = current->type;
	new_current->parts = duplicate_word_parts(current->parts);
	new_current->next = NULL;
	return (new_current);
}

// Ajoute un token à la liste chaînée
static void	add_token_with_prev(t_token **new_tokens, t_token **prev,
								t_token *new_current)
{
	if (!*new_tokens)
		*new_tokens = new_current;
	else
		(*prev)->next = new_current;
	*prev = new_current;
}

t_token	*duplicate_tokens(t_token *tokens)
{
	t_token	*new_tokens;
	t_token	*current;
	t_token	*new_current;
	t_token	*prev;

	if (!tokens)
		return (NULL);
	new_tokens = NULL;
	prev = NULL;
	current = tokens;
	while (current)
	{
		new_current = create_new_token(current);
		if (!new_current)
		{
			if (new_tokens)
				free_tokens(new_tokens);
			return (NULL);
		}
		add_token_with_prev(&new_tokens, &prev, new_current);
		current = current->next;
	}
	return (new_tokens);
}

// Crée un token jusqu'à une position donnée
t_token	*create_token_until_pos(t_token *current, int pos, int i)
{
	t_token	*new_current;

	if (i >= pos)
		return (NULL);
	new_current = malloc(sizeof(t_token));
	if (!new_current)
		return (NULL);
	if (!copy_token_value(new_current, current))
	{
		free(new_current);
		return (NULL);
	}
	new_current->type = current->type;
	new_current->parts = duplicate_word_parts(current->parts);
	new_current->next = NULL;
	return (new_current);
}
