/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:19:22 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_redir_only(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	(void)envp;
	(void)shell;
	init_redirect(&red);
	if (!node || !node->redirections)
		return (0);
	process_all_heredocs(node);
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	if (node->left)
		node->left->redirections = node->redirections;
	if (node->right)
		node->right->redirections = node->redirections;
	close_redirect_fds(&red);
	restore_std_fds(&red);
	if (node->left && (node->left->type == NODE_AND
			|| node->left->type == NODE_OR))
		execute_ast(node->left, envp, shell);
	return (0);
}

static void	child_exec_process(t_node *node, char **envp)
{
	t_redirect	red;

	init_redirect(&red);
	if (node->redirections && !apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		exit(1);
	}
	exit(exec_external(node->cmd, envp));
}

int	exec_cmd_with_redirections(t_node *node, char **envp)
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
		child_exec_process(node, envp);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_cmd_node(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	if ((!node->cmd || !node->cmd[0]) && node->redirections)
		return (execute_redir_only(node, envp, shell));
	if (!node->cmd || !node->cmd[0])
		return (0);
	return (process_cmd_tokens(node, envp, shell));
}
