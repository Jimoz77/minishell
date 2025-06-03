/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/02 23:11:43 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	exec_pipe_left(t_node *node, char ***envp, int pipefd[2],
	t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		exit(execute_node_by_type(node->left, envp, shell));
	}
	return (pid);
}

static int	exec_pipe_right(t_node *node, char ***envp, int pipefd[2],
	t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		exit(execute_node_by_type(node->right, envp, shell));
	}
	return (pid);
}

static int	wait_for_pipes(pid_t pid_left, pid_t pid_right)
{
	int	status;

	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_pipe_node(t_node *node, char ***envp, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	pid_left = exec_pipe_left(node, envp, pipefd, shell);
	if (pid_left == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	pid_right = exec_pipe_right(node, envp, pipefd, shell);
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid_right == -1)
		return (1);
	return (wait_for_pipes(pid_left, pid_right));
}

