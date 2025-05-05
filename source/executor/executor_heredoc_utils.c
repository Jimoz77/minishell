/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:48:36 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 16:48:25 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initialise le tableau heredocs de t_shell
int	init_heredocs(t_shell *shell, int count)
{
	shell->heredocs = ft_calloc(count + 1, sizeof(char *));
	if (!shell->heredocs)
	{
		perror("minishell: heredoc alloc");
		return (0);
	}
	return (1);
}

// Libère le contenu du tableau heredocs de t_shell
void	free_heredocs(t_shell *shell)
{
	int	i;

	if (!shell->heredocs)
		return ;
	i = 0;
	while (shell->heredocs[i])
	{
		free(shell->heredocs[i]);
		i++;
	}
	free(shell->heredocs);
	shell->heredocs = NULL;
}

// Convertit une chaîne en pipe pour injection dans stdin
int	string_to_fd(const char *content)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("minishell: pipe");
		return (-1);
	}
	if (write(pipe_fd[1], content, ft_strlen(content)) == -1)
	{
		perror("minishell: write to pipe");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}
