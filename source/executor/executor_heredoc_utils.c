/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 17:29:33 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/17 15:57:04 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	interupt(char *line, t_shell *shell)
{
	g_signal = SIGNAL_NORMAL;
	set_heredoc_mode(0);
	if (shell)
	{
		shell->exit_status = 130;
		shell->heredoc_interrupted = 1;
	}
	if (line)
		free(line);
	return (0);
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

int	process_heredoc_line(const char *delimiter, t_shell *shell,
							char **content, size_t delim_len)
{
	char	*line;
	char	*expanded_line;

	set_heredoc_mode(1);
	line = readline("> ");
	if (g_signal == SIGNAL_HEREDOC_INTERRUPTED)
		return (interupt(line, shell));
	set_heredoc_mode(0);
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
