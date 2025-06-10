/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:20:28 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:01:13 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_file_error(char *filename, int error_code)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(filename, 2);
	ft_putstr_fd(": ", 2);
	if (error_code == EISDIR)
		ft_putendl_fd("Is a directory", 2);
	else if (error_code == EACCES)
		ft_putendl_fd("Permission denied", 2);
	else if (error_code == ENOENT)
		ft_putendl_fd("No such file or directory", 2);
	else
		ft_putendl_fd(strerror(error_code), 2);
}

static int	apply_redirect_in(t_redir *redir, t_redirect *red)
{
	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
	red->stdin_fd = open(redir->filename, O_RDONLY);
	if (red->stdin_fd == -1)
	{
		print_file_error(redir->filename, errno);
		return (0);
	}
	return (1);
}

static int	apply_redirect_out(t_redir *redir, t_redirect *red)
{
	int	flags;

	if (red->stdout_fd != -1)
	{
		close(red->stdout_fd);
		red->stdout_fd = -1;
	}
	flags = O_WRONLY | O_CREAT;
	if (redir->type == TOKEN_REDIRECT_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	red->stdout_fd = open(redir->filename, flags, 0644);
	if (red->stdout_fd == -1)
	{
		print_file_error(redir->filename, errno);
		return (0);
	}
	return (1);
}

static int	apply_single_redirection(t_redir *redir, t_redirect *red,
	t_node *node)
{
	if (redir->type == TOKEN_REDIRECT_IN)
		return (apply_redirect_in(redir, red));
	else if (redir->type == TOKEN_REDIRECT_OUT || redir->type == TOKEN_APPEND)
		return (apply_redirect_out(redir, red));
	else if (redir->type == TOKEN_HEREDOC)
		return (apply_heredoc_redir(node, redir->filename, red));
	return (1);
}

int	apply_node_redirections(t_node *node, t_redirect *red)
{
	t_redir	*current;

	if (!node || !node->redirections)
		return (1);
	current = node->redirections;
	while (current)
	{
		if (!apply_single_redirection(current, red, node))
			return (0);
		current = current->next;
	}
	if (red->stdin_fd != -1 && dup2(red->stdin_fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (0);
	}
	if (red->stdout_fd != -1 && dup2(red->stdout_fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		return (0);
	}
	return (1);
}
