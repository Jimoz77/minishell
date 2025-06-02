/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:40:45 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 23:14:41 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*read_heredoc_input(const char *delimiter)
{
	char	*line;
	char	*content;
	char	*new_content;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		new_content = ft_strjoin(content, line);
		free(content);
		free(line);
		content = ft_strjoin(new_content, "\n");
		free(new_content);
		if (!content)
			return (NULL);
	}
	return (content);
}

static void	process_node_heredocs(t_node *node)
{
	t_heredoc	*heredoc;

	if (!node || !node->heredocs)
		return ;
	heredoc = node->heredocs;
	while (heredoc)
	{
		if (!heredoc->processed && heredoc->delimiter)
		{
			heredoc->content = read_heredoc_input(heredoc->delimiter);
			if (heredoc->content)
			{
				heredoc->pipe_fd = string_to_fd(heredoc->content);
				if (heredoc->pipe_fd == -1)
				{
					free(heredoc->content);
					heredoc->content = NULL;
				}
			}
			heredoc->processed = 1;
		}
		heredoc = heredoc->next;
	}
}

void	process_all_heredocs(t_node *node)
{
	if (!node)
		return ;
	// Parcourir tout l'AST et traiter tous les heredocs
	process_node_heredocs(node);
	process_all_heredocs(node->left);
	process_all_heredocs(node->right);
}

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
		if (heredoc->delimiter && ft_strncmp(heredoc->delimiter, delimiter,
			ft_strlen(delimiter) + 1) == 0 && heredoc->processed 
			&& heredoc->pipe_fd != -1)
		{
			red->stdin_fd = dup(heredoc->pipe_fd);
			if (red->stdin_fd == -1)
			{
				perror("minishell: dup");
				return (0);
			}
			return (1);
		}
		heredoc = heredoc->next;
	}
	ft_putstr_fd("minishell: warning: heredoc missing\n", 2);
	return (0);
}
