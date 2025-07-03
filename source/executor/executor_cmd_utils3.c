/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:17:11 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/03 20:50:02 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*find_node_tokens(t_node *node, t_shell *shell)
{
	t_token	*current;

	if (!node->cmd || !node->cmd[0] || !shell->tokens)
		return (NULL);
	current = shell->tokens;
	if (shell->last_used_token)
	{
		while (current && current != shell->last_used_token)
			current = current->next;
		if (current)
			current = current->next;
	}
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value
			&& ft_strcmp(current->value, node->cmd[0]) == 0)
		{
			shell->last_used_token = current;
			return (current);
		}
		current = current->next;
	}
	return (NULL);
}

int	process_cmd_expansion(t_token *temp_tokens, t_shell *shell)
{
	t_token	*current;
	char	*unquoted_value;

	current = temp_tokens;
	while (current)
	{
		if (!current->parts || (current->parts
				&& current->parts->type != QUOTE_SINGLE))
		{
			shell->tokens = temp_tokens;
			scan_envar_execution_phase(shell, current);
		}
		if (current->parts)
		{
			unquoted_value = build_unquoted_value(current->parts);
			if (unquoted_value)
			{
				free(current->value);
				current->value = unquoted_value;
			}
		}
		current = current->next;
	}
	return (1);
}

int	handle_no_tokens_case(t_node *node, char ***envp, t_shell *shell)
{
	if (ft_is_builtin(node->cmd, envp))
		return (exec_builtin_with_redirections(node, envp));
	else
		return (exec_cmd_with_redirections(node, *envp, shell));
}

int	exec_external(char **cmd, char **envp)
{
	char	*path;
	int		special;

	if (!cmd || !cmd[0])
		return (1);
	special = handle_special_commands(cmd);
	if (special)
		return (special);
	path = handle_direct_path(cmd[0]);
	if (!path)
		path = ft_path_finder(cmd[0], &envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	execve(path, cmd, envp);
	perror("minishell: execve");
	free(path);
	return (126);
}

int	exec_builtin_with_redirections(t_node *node, char ***envp)
{
	t_redirect	red;
	int			result;

	if (!node->redirections)
		return (execute_builtin(node->cmd, envp));
	init_redirect(&red);
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	result = execute_builtin(node->cmd, envp);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (result);
}
