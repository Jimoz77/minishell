/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils4.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 20:45:02 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 20:47:01 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	expand_tokens_variables(t_token *cmd_tokens, t_shell *shell)
{
	t_token	*current;

	current = cmd_tokens;
	while (current)
	{
		scan_envar_execution_phase(shell, current);
		current = current->next;
	}
}

static void	process_token_quotes_expansion(t_token *current)
{
	char	*unquoted_value;

	if (current->parts)
	{
		unquoted_value = build_unquoted_value(current->parts);
		if (unquoted_value)
		{
			free(current->value);
			current->value = unquoted_value;
		}
	}
}

static void	process_tokens_quotes(t_token *cmd_tokens)
{
	t_token	*current;

	current = cmd_tokens;
	while (current && current->type == TOKEN_WORD)
	{
		process_token_quotes_expansion(current);
		current = current->next;
	}
}

static int	expand_cmd_with_original_tokens(t_node *node, t_shell *shell)
{
	t_token	*cmd_tokens;
	t_token	*original_tokens;

	cmd_tokens = find_node_tokens(node, shell);
	if (!cmd_tokens)
		return (-1);
	original_tokens = shell->tokens;
	expand_tokens_variables(cmd_tokens, shell);
	process_tokens_quotes(cmd_tokens);
	shell->tokens = original_tokens;
	update_cmd_from_tokens(node->cmd, cmd_tokens);
	return (0);
}

int	process_cmd_tokens(t_node *node, char ***envp, t_shell *shell)
{
	t_process_cmd_tokens	v;

	if (expand_cmd_with_original_tokens(node, shell) == 0)
	{
		if (ft_is_builtin(node->cmd, envp))
			return (exec_builtin_with_redirections(node, envp));
		else
			return (exec_cmd_with_redirections(node, *envp, shell));
	}
	v.original_tokens = shell->tokens;
	v.temp_tokens = create_tokens_from_cmd(node->cmd, shell);
	if (!v.temp_tokens)
	{
		shell->tokens = v.original_tokens;
		return (handle_no_tokens_case(node, envp, shell));
	}
	process_cmd_expansion(v.temp_tokens, shell);
	shell->tokens = v.original_tokens;
	update_cmd_from_tokens(node->cmd, v.temp_tokens);
	if (ft_is_builtin(node->cmd, envp))
		v.result = exec_builtin_with_redirections(node, envp);
	else
		v.result = exec_cmd_with_redirections(node, *envp, shell);
	free_tokens(v.temp_tokens);
	return (v.result);
}
