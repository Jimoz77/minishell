/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 23:17:02 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/01 16:02:47 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*find_temp_token(t_shell *shell, int i)
{
	t_token	*temp;
	int		j;

	if (!shell || !shell->tokens)
		return (NULL);
	temp = shell->tokens;
	j = 0;
	while (temp && j < i)
	{
		temp = temp->next;
		j++;
	}
	return (temp);
}

static void	setup_single_quote_parts(t_token *current)
{
	current->parts = malloc(sizeof(t_word_part));
	if (current->parts)
	{
		current->parts->type = QUOTE_SINGLE;
		current->parts->content = NULL;
		current->parts->next = NULL;
	}
}

static int	setup_token_parts(t_token *current, t_shell *shell, int i)
{
	t_token	*temp;

	temp = find_temp_token(shell, i);
	if (temp && temp->parts && temp->parts->type == QUOTE_SINGLE)
		setup_single_quote_parts(current);
	return (1);
}

static t_token	*create_single_token(char *cmd_arg, t_shell *shell, int i)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(cmd_arg);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = TOKEN_WORD;
	token->parts = NULL;
	token->next = NULL;
	setup_token_parts(token, shell, i);
	return (token);
}

static int	is_logical_operator(t_token *token)
{
	return (token->type == TOKEN_AND || token->type == TOKEN_OR);
}

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

static t_token	*find_original_token_for_cmd(t_shell *shell, char *cmd_value,
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

static void	copy_original_parts(t_token *new_token, t_token *original)
{
	if (original && original->parts)
	{
		if (new_token->parts)
			free_word_parts(new_token->parts);
		new_token->parts = duplicate_word_parts(original->parts);
	}
}

static void	link_token(t_token **tokens, t_token **current, t_token *new_token)
{
	if (!*tokens)
		*tokens = new_token;
	else
		(*current)->next = new_token;
	*current = new_token;
}

t_token	*create_tokens_from_cmd(char **cmd, t_shell *shell)
{
	t_token	*tokens;
	t_token	*current;
	t_token	*new_token;
	t_token	*original;
	int		i;

	i = 0;
	tokens = NULL;
	current = NULL;
	while (cmd[i])
	{
		new_token = create_single_token(cmd[i], shell, i);
		if (!new_token)
			return (free_tokens(tokens), NULL);
		original = find_original_token_for_cmd(shell, cmd[i], i);
		copy_original_parts(new_token, original);
		link_token(&tokens, &current, new_token);
		i++;
	}
	return (tokens);
}

void	init_token_from_cmd(t_token *token, char *cmd_arg, t_shell *shell)
{
	token->value = ft_strdup(cmd_arg);
	token->type = TOKEN_WORD;
	token->parts = NULL;
	if (shell->tokens && shell->tokens->parts
		&& shell->tokens->parts->type == QUOTE_SINGLE)
	{
		token->parts = malloc(sizeof(t_word_part));
		if (token->parts)
			token->parts->type = QUOTE_SINGLE;
	}
	token->next = NULL;
}

void	process_token_expansion(t_shell *shell, t_token *temp_tokens)
{
	while (temp_tokens)
	{
		if (!temp_tokens->parts || (temp_tokens->parts
				&& temp_tokens->parts->type != QUOTE_SINGLE))
			scan_envar_execution_phase(shell, temp_tokens);
		temp_tokens = temp_tokens->next;
	}
}
