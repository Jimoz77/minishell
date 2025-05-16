/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/14 20:57:38 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_pipe_node_right(t_node *node,\
	char ***envp, int pipefd[2], pid_t pid_left)
{
	int		status;
	pid_t	pid_right;

	// Processus droit (lecture)
	pid_right = fork();
	if (pid_right == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		exit(execute_node_by_type(node->right, envp));
	}
	// Nettoyage parent
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	return (WEXITSTATUS(status));
}

int	execute_pipe_node(t_node *node, char ***envp)
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
		exit(execute_node_by_type(node->left, envp));
	}
	return (execute_pipe_node_right(node, envp, pipefd, pid_left));
}
