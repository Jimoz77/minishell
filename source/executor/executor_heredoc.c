/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:40:45 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/09 17:16:48 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*append_line_to_content(char *content, char *line)
{
	char	*tmp;
	char	*new_content;

	tmp = ft_strjoin(content, line);
	free(content);
	if (!tmp)
		return (NULL);
	new_content = ft_strjoin(tmp, "\n");
	free(tmp);
	return (new_content);
}

static int	process_heredoc_line(const char *delimiter, t_shell *shell,
							char **content, size_t delim_len)
{
	char	*line;
	char	*expanded_line;

	line = readline("> ");
	if (!line)
		return (0);
	if (ft_strlen(line) == delim_len
		&& ft_strncmp(line, delimiter, delim_len) == 0)
	{
		free(line);
		return (0);
	}
	expanded_line = expand_heredoc_line(line, shell);
	*content = append_line_to_content(*content, expanded_line);
	free(expanded_line);
	if (!*content)
		return (-1);
	return (1);
}

static char	*read_heredoc_input_expanded(const char *delimiter,
										t_shell *shell)
{
	char	*content;
	size_t	delim_len;
	int		result;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	delim_len = ft_strlen(delimiter);
	while (1)
	{
		result = process_heredoc_line(delimiter, shell, &content, delim_len);
		if (result == 0)
			break ;
		if (result == -1)
			return (NULL);
	}
	return (content);
}

static void	process_node_heredocs_expanded(t_node *node, t_shell *shell)
{
	t_heredoc	*heredoc;

	if (!node || !node->heredocs)
		return ;
	heredoc = node->heredocs;
	while (heredoc)
	{
		if (!heredoc->processed && heredoc->delimiter)
		{
			heredoc->content = read_heredoc_input_expanded(heredoc->delimiter,
					shell);
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

void	process_all_heredocs_expanded(t_node *node, t_shell *shell)
{
	if (!node)
		return ;
	process_node_heredocs_expanded(node, shell);
	process_all_heredocs_expanded(node->left, shell);
	process_all_heredocs_expanded(node->right, shell);
}
