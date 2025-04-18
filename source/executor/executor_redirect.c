/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:56:01 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/18 13:01:35 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ouvre le fichier approprié selon le type de redirection
static int	open_redirect_file(t_node *node)
{
	int		fd;
	char	*filename;

	fd = -1;
	filename = get_unquoted_filename(node);
	if (!filename)
		return (-1);
	if (node->type == NODE_REDIRECT_OUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->type == NODE_APPEND)
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (node->type == NODE_REDIRECT_IN)
		fd = open(filename, O_RDONLY);
	if (fd == -1)
		perror("minishell");
	free(filename);
	return (fd);
}

// Configure les redirections dans le processus enfant
static void	setup_redirection(t_node *node, int fd)
{
	int	std_fd;

	if (node->type == NODE_REDIRECT_OUT || node->type == NODE_APPEND)
		std_fd = STDOUT_FILENO;
	else
		std_fd = STDIN_FILENO;
	if (dup2(fd, std_fd) == -1)
	{
		perror("minishell: dup2");
		exit(1);
	}
	close(fd);
}

// Execute la commande avec redirection dans un processus enfant
static void	exec_with_redirect(t_node *node, char **envp, int fd)
{
	// Restaurer les gestionnaires de signaux par défaut
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	setup_redirection(node, fd);
	exit(execute_ast(node->left, envp));
}

// Gère les erreurs de fork et ferme les fd
static int	handle_fork_error(int fd)
{
	perror("minishell: fork");
	close(fd);
	return (1);
}

// Gère les redirections simple < , > , >>
int	execute_redirect_node(t_node *node, char **envp)
{
	int		status;
	pid_t	pid;
	int		fd;

	fd = open_redirect_file(node);
	if (fd == -1)
		return (1);
	pid = fork();
	if (pid == 0)
		exec_with_redirect(node, envp, fd);
	else if (pid > 0)
	{
		close(fd);
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else
		handle_fork_error(fd);
	return (0);
}
