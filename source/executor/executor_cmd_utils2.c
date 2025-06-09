/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 23:17:02 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/09 20:00:59 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*create_tokens_from_cmd(char **cmd, t_shell *shell)
{
	t_token	*tokens;
	t_token	*current;
	int		i;

	tokens = NULL;
	i = 0;
	while (cmd[i])
	{
		if (tokens == NULL)
		{
			tokens = malloc(sizeof(t_token));
			current = tokens;
		}
		else
		{
			current->next = malloc(sizeof(t_token));
			current = current->next;
		}
		if (!current)
			return (free_tokens(tokens), NULL);
		
		current->value = ft_strdup(cmd[i]);
		if(!current->value)
		{
			free_tokens(tokens);
			return (NULL);
		}
		current->type = TOKEN_WORD;
		current->parts = NULL;
		
		// Ne pas accéder à shell->tokens si NULL
		if (shell && shell->tokens)
		{
			t_token *temp = shell->tokens;
			int j = 0;
			// Trouver le token correspondant à cmd[i]
			while (temp && j < i)
			{
				temp = temp->next;
				j++;
			}
			// Si on a trouvé le token et qu'il a des parts avec QUOTE_SINGLE
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
		}
		
		current->next = NULL;
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

void	update_cmd_from_tokens(char **cmd, t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && cmd[i])
	{
		free(cmd[i]);
		cmd[i] = ft_strdup(current->value);
		current = current->next;
		i++;
	}
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
