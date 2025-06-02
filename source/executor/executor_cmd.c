/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 23:11:22 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	exec_external(char **cmd, char **envp)
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
	return (execute_with_path(path, cmd, envp));
}

static int	exec_forked(t_node *node, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
		exit(exec_external(node->cmd, envp));
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static void	apply_node_redirections_forked(t_node *node, t_redirect *red)
{
	if (node->redirections && !apply_node_redirections(node, red))
	{
		close_redirect_fds(red);
		restore_std_fds(red);
		exit(1);
	}
}

static int	exec_cmd_with_redirections(t_node *node, char **envp,
	t_shell *shell)
{
	t_redirect	red;
	pid_t		pid;
	int			status;

	if (!node->redirections)
		return (exec_forked(node, envp));
	init_redirect(&red);
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
	{
		apply_node_redirections_forked(node, &red);
		exit(exec_external(node->cmd, envp));
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// Reste du fichier avec la logique d'expansion inchangée
int	execute_cmd_node(t_node *node, char ***envp, t_shell *shell)
{
	t_token	*original_tokens;
	t_token	*temp_tokens;
	int		result;

	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	original_tokens = shell->tokens;
	temp_tokens = create_tokens_from_cmd(node->cmd, shell);
	if (!temp_tokens)
		return (1);
	shell->tokens = temp_tokens;
	while(temp_tokens)
	{
		if (!temp_tokens->parts || (temp_tokens->parts 
			&& temp_tokens->parts->type != QUOTE_SINGLE))
			scan_envar_execution_phase(shell, temp_tokens);
		temp_tokens = temp_tokens->next;
	}
	update_cmd_from_tokens(node->cmd, shell->tokens);
	free_tokens(shell->tokens);
	shell->tokens = original_tokens;
	if (ft_is_builtin(node->cmd, envp))
		result = execute_builtin(node->cmd, envp);
	else
		result = exec_cmd_with_redirections(node, *envp, shell);
	return (result);
}
