/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:40:45 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:29:18 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*append_line(char *content, char *line)
{
	char	*tmp;
	char	*new_content;

	tmp = ft_strjoin(content, line);
	free(content);
	free(line);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "\n");
	free(tmp);
	return (new_content);
}

static char	*read_heredoc_input(const char *delimiter)
{
	char	*line;
	char	*content;
	size_t	delim_len;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strlen(line) == delim_len
			&& ft_strncmp(line, delimiter, delim_len) == 0)
		{
			free(line);
			break ;
		}
		content = append_line(content, line);
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
	process_node_heredocs(node);
	process_all_heredocs(node->left);
	process_all_heredocs(node->right);
}
