/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:39:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/08 15:37:28 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_envar_and_unquote(t_token *tokens, t_shell *shell)
{
	t_token	*cur;
	char	*unquoted_value;

	cur = tokens;
	while (cur)
	{
		if (!cur->parts || cur->parts->type != QUOTE_SINGLE)
			scan_envar_execution_phase(shell, cur);
		cur = cur->next;
	}
	cur = tokens;
	while (cur)
	{
		if (cur->parts)
		{
			unquoted_value = build_unquoted_value(cur->parts);
			if (unquoted_value)
			{
				free(cur->value);
				cur->value = unquoted_value;
			}
		}
		cur = cur->next;
	}
}

int	prepare_cmd_tokens(t_node *node, t_shell *shell)
{
	t_token	*original;
	t_token	*tokens;

	original = shell->tokens;
	tokens = create_tokens_from_cmd(node->cmd, shell);
	if (!tokens)
		return (-1);
	shell->tokens = tokens;
	process_envar_and_unquote(tokens, shell);
	shell->tokens = original;
	update_cmd_from_tokens(node->cmd, tokens);
	free_tokens(tokens);
	return (0);
}

int	execute_cmd_builtin_or_exec(t_node *node, char ***envp, t_redirect *red)
{
	char	*path;
	int		result;

	if (ft_is_builtin(node->cmd, envp))
	{
		result = execute_builtin(node->cmd, envp);
		close_redirect_fds(red);
		restore_std_fds(red);
		return (result);
	}
	path = handle_direct_path(node->cmd[0]);
	if (!path)
		path = ft_path_finder(node->cmd[0], envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	execve(path, node->cmd, *envp);
	perror("minishell: execve");
	free(path);
	return (126);
}
