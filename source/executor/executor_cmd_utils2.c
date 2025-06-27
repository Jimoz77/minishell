/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 23:17:02 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/19 19:28:13 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	setup_token_parts(t_token *current, t_shell *shell, int i)
{
	t_token	*temp;
	int		j;

	if (!shell || !shell->tokens)
		return (1);
	temp = shell->tokens;
	j = 0;
	while (temp && j < i)
	{
		temp = temp->next;
		j++;
	}
	if (temp && temp->parts && temp->parts->type == QUOTE_SINGLE)
	{
		current->parts = malloc(sizeof(t_word_part));
		if (current->parts)
		{
			current->parts->type = QUOTE_SINGLE;
			current->parts->content = NULL;
			current->parts->next = NULL;
		}
	}
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

static t_token	*find_original_token_for_cmd(t_shell *shell, char *cmd_value, int position)
{
	t_token	*current;
	int		word_count;
	int		after_logical;
	
	current = shell->tokens;
	word_count = 0;
	after_logical = 0;
	
	while (current)
	{
		if (current->type == TOKEN_AND || current->type == TOKEN_OR)
		{
			after_logical = 1;
			word_count = 0;
		}
		else if (current->type == TOKEN_WORD)
		{
			if (after_logical && word_count == position)
				return (current);
			else if (!after_logical && current->value && 
					ft_strcmp(current->value, cmd_value) == 0)
				return (current);
			word_count++;
		}
		current = current->next;
	}
	return (NULL);
}

t_token	*create_tokens_from_cmd(char **cmd, t_shell *shell)
{
	t_token	*tokens;
	t_token	*current;
	t_token	*new_token;
	t_token	*original;
	int		i;

	printf("DEBUG: create_tokens_from_cmd called with:\n");
	i = 0;
	while (cmd[i])
	{
		printf("  cmd[%d] = '%s'\n", i, cmd[i]);
		i++;
	}

	tokens = NULL;
	current = NULL;
	i = 0;
	while (cmd[i])
	{
		new_token = create_single_token(cmd[i], shell, i);
		if (!new_token)
			return (free_tokens(tokens), NULL);
			
		original = find_original_token_for_cmd(shell, cmd[i], i);
		if (original)
		{
			printf("DEBUG: Found original token for '%s': value='%s', has_parts=%d\n", 
				cmd[i], original->value, original->parts ? 1 : 0);
			if (original->parts)
			{
				if (new_token->parts)
					free_word_parts(new_token->parts);
				new_token->parts = duplicate_word_parts(original->parts);
			}
		}
		else
		{
			printf("DEBUG: No original token found for '%s'\n", cmd[i]);
		}
		
		if (!tokens)
			tokens = new_token;
		else
			current->next = new_token;
		current = new_token;
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
