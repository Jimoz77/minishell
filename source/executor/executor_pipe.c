/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/21 14:35:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_pipe_node_right(t_node *node,\
	char ***envp, int pipefd[2], pid_t pid_left, t_shell *shell)
{
	int		status;
	pid_t	pid_right;

	pid_right = fork();
	if (pid_right == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(pipefd[0]);
		exit(execute_node_by_type(node->right, envp, shell));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);  // Ignorer le statut du process gauche
	waitpid(pid_right, &status, 0);  // Utiliser le statut du dernier process
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

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), -1);
	// Processus gauche (écriture)
	pid_left = fork();
	if (pid_left == 0)
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
	return (execute_pipe_node_right(node, envp, pipefd, pid_left, shell));
}
