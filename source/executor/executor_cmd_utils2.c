/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 23:17:02 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/03 21:25:50 by jimpa            ###   ########.fr       */
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

t_token	*create_single_token(char *cmd_arg, t_shell *shell, int i)
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

int	is_logical_operator(t_token *token)
{
	return (token->type == TOKEN_AND || token->type == TOKEN_OR);
}
