/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:08:57 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:09:14 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	finalize_token_value(t_token *token, char *new_val)
{
	char	*cleaned;

	cleaned = clean_double_slashes(new_val);
	free(new_val);
	free(token->value);
	token->value = cleaned;
}

static void	process_variable(t_shell *shell, t_token *token)
{
	char	*new_val;

	new_val = ft_strdup("");
	process_character_loop(shell, token, &new_val);
	finalize_token_value(token, new_val);
	process_parts(shell, token);
}

void	envar_to_value(t_shell *shell, t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
		return ;
	if (token->value)
		process_variable(shell, token);
}

int	scan_envar_parsing_phase(t_shell *shell)
{
	int		found;
	t_token	*tmp;

	found = 0;
	tmp = shell->tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->value)
		{
			if (tmp->value[0] == '$' && tmp->value[1] == '$')
			{
				free(tmp->value);
				tmp->value = ft_itoa(get_shell_pid());
				found = 1;
			}
		}
		else if (is_redirection(tmp->type)
			&& tmp->next && tmp->next->type == TOKEN_WORD)
		{
			envar_to_value(shell, tmp->next);
			found = 1;
		}
		tmp = tmp->next;
	}
	return (found);
}

// Fonction d'expansion complète à l'exécution
int	scan_envar_execution_phase(t_shell *shell, t_token *tokens)
{
	int		found;
	t_token	*tmp;

	found = 0;
	tmp = tokens;
	if (tmp->type == TOKEN_WORD && tmp->value)
	{
		if (tmp->value[0] == '$' && tmp->value[1] == '?')
		{
			free(tmp->value);
			tmp->value = ft_itoa(shell->exit_status);
			found = 1;
		}
		else
		{
			envar_to_value(shell, tmp);
		}
	}
	return (found);
}
