/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:29:33 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/19 18:59:14 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	string_to_fd(const char *content)
{
	int	pipe_fd[2];

	if (!content)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	if (write(pipe_fd[1], content, ft_strlen(content)) == -1)
	{
		perror("minishell: write");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

int	apply_heredoc_redir(t_node *node, char *delimiter, t_redirect *red)
{
	t_heredoc	*heredoc;

	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
	heredoc = node->heredocs;
	while (heredoc)
	{
		if (heredoc->delimiter
			&& ft_strncmp(heredoc->delimiter, delimiter,
				ft_strlen(delimiter) + 1) == 0
			&& heredoc->processed && heredoc->pipe_fd != -1)
		{
			red->stdin_fd = heredoc->pipe_fd;
			return (1);
		}
		heredoc = heredoc->next;
	}
	ft_putstr_fd("minishell: warning: heredoc missing\n", 2);
	return (0);
}
