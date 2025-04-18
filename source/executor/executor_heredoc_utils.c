/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:48:36 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/16 13:22:27 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Gestionnaire pour SIGINT pendant les heredocs
void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_signal = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
	exit(130);
}

// Lit l'entrée jusqu'au delimiter et l'écrit dans le pipe
void	read_heredoc_input(int pipe_fd, char *delimiter)
{
	char				*line;
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_heredoc_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(1, "\n", 1);
			break;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		write(pipe_fd, line, ft_strlen(line));
		write(pipe_fd, "\n", 1);
		free(line);
	}
}

// Vérifie la structure du noeud pour heredoc
int	check_heredoc_node(t_node *node)
{
	if (!node || !node->right || !node->right->cmd || !node->right->cmd[0])
	{
		ft_printf("minishell: heredoc: invalid node structure\n");
		return (0);
	}
	return (1);
}

// Gère le processus parent après le fork pour heredoc
int	handle_parent_process(int *pipe_fd, pid_t pid)
{
	int	status;
	int	fd_out;

	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_signal = 130;
		close(pipe_fd[0]);
		setup_signals();
		return (-1);
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		setup_signals();
		return (-1);
	}
	fd_out = pipe_fd[0];
	setup_signals();
	return (fd_out);
}

// Gère un seul heredoc et retourne un fd pour lire son contenu
int	process_single_heredoc(t_node *node)
{
	int		pipe_fd[2];
	pid_t	pid;

	if (!check_heredoc_node(node))
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		close(pipe_fd[0]);
		read_heredoc_input(pipe_fd[1], node->right->cmd[0]);
		close(pipe_fd[1]);
		exit(0);
	}
	else if (pid < 0)
		return (handle_heredoc_error(pipe_fd, "minishell: fork"));
	return (handle_parent_process(pipe_fd, pid));
}
