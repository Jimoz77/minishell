/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:35:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/16 13:22:11 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Gère les erreurs de fork et ferme les fd
int	handle_heredoc_error(int *pipe_fd, char *error_msg)
{
	perror(error_msg);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (1);
}

// Exécute les commandes dans le processus enfant avec l'entrée du heredoc
static int	exec_in_child(t_node *node, int heredoc_fd, char **envp)
{
	signal(SIGINT, SIG_DFL);
	dup2(heredoc_fd, STDIN_FILENO);
	close(heredoc_fd);
	exit(execute_ast(node->left, envp));
}

// Gère le résultat du waitpid pour le processus d'exécution
static int	handle_execution_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// Crée un processus enfant pour exécuter avec le heredoc
static int	fork_executor(t_node *node, int heredoc_fd, char **envp)
{
	pid_t	pid_executor;
	int		status;

	pid_executor = fork();
	if (pid_executor == 0)
		exec_in_child(node, heredoc_fd, envp);
	else if (pid_executor < 0)
	{
		perror("minishell: fork");
		close(heredoc_fd);
		return (1);
	}
	close(heredoc_fd);
	waitpid(pid_executor, &status, 0);
	return (handle_execution_status(status));
}

// Gère les here_doc (pour nœud unique)
int	handle_heredoc(t_node *node, char **envp)
{
	int	heredoc_fd;

	heredoc_fd = process_single_heredoc(node);
	if (heredoc_fd == -1)
		return (1);
	return (fork_executor(node, heredoc_fd, envp));
}
