/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:34:24 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	child_paren_exec(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	init_redirect(&red);
	if (node->redirections && !apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		exit(1);
	}
	exit(execute_ast(node->left, envp, shell));
}

static int	fork_and_execute(t_node *node, char ***envp, t_shell *shell)
{
	pid_t	child_pid;
	int		status;

	child_pid = fork();
	if (child_pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (child_pid == 0)
		child_paren_exec(node, envp, shell);
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_paren_node(t_node *node, char ***envp, t_shell *shell)
{
	if (!node || !node->left)
		return (1);
	if (!node->redirections)
		return (execute_ast(node->left, envp, shell));
	return (fork_and_execute(node, envp, shell));
}
