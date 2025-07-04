/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 17:41:44 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_simple_command(t_node *node)
{
	return (node && node->type == NODE_CMD && node->cmd && node->cmd[0]);
}

int	execute_in_pipe_context(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	if (!node)
		return (1);
	if (!is_simple_command(node))
		return (execute_node_by_type(node, envp, shell));
	if (prepare_cmd_tokens(node, shell) == -1)
		return (1);
	init_redirect(&red);
	if (node->redirections && !apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	return (execute_cmd_builtin_or_exec(node, envp, &red));
}

void	execute_pipe_child_left(t_node *node, char ***envp,
			t_shell *shell, int pipefd[2])
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	exit(execute_in_pipe_context(node->left, envp, shell));
}

void	execute_pipe_child_right(t_node *node, char ***envp,
			t_shell *shell, int pipefd[2])
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	exit(execute_in_pipe_context(node->right, envp, shell));
}

int	execute_pipe_node(t_node *node, char ***envp, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), 1);
	pid_left = fork();
	if (pid_left == 0)
		execute_pipe_child_left(node, envp, shell, pipefd);
	pid_right = fork();
	if (pid_right == 0)
		execute_pipe_child_right(node, envp, shell, pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
