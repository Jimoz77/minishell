/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils5.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:55:16 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 21:29:20 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	should_return_current_token(t_search_context var)
{
	if (var.current->type != TOKEN_WORD)
		return (0);
	if (var.after_logical && var.word_count == var.position)
		return (1);
	if (!var.after_logical && var.current->value
		&& ft_strcmp(var.current->value, var.cmd_value) == 0)
		return (1);
	return (0);
}

static void	update_counters(t_token *current,
	int *word_count, int *after_logical)
{
	if (is_logical_operator(current))
	{
		*after_logical = 1;
		*word_count = 0;
	}
	else if (current->type == TOKEN_WORD)
		(*word_count)++;
}

t_token	*find_original_token_for_cmd(t_shell *shell, char *cmd_value,
										int position)
{
	t_search_context	var;

	var.current = shell->tokens;
	var.cmd_value = cmd_value;
	var.word_count = 0;
	var.after_logical = 0;
	var.position = position;
	while (var.current)
	{
		if (should_return_current_token(var))
			return (var.current);
		update_counters(var.current, &var.word_count, &var.after_logical);
		var.current = var.current->next;
	}
	return (NULL);
}

void	copy_original_parts(t_token *new_token, t_token *original)
{
	if (original && original->parts)
	{
		if (new_token->parts)
			free_word_parts(new_token->parts);
		new_token->parts = duplicate_word_parts(original->parts);
	}
}

void	link_token(t_token **tokens, t_token **current, t_token *new_token)
{
	if (!*tokens)
		*tokens = new_token;
	else
		(*current)->next = new_token;
	*current = new_token;
}
