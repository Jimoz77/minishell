/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_fork.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 14:53:26 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/17 15:57:54 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Fonction auxiliaire qui gère les résultats du traitement du heredoc */
static void	handle_heredoc_result(t_heredoc *heredoc, t_shell *shell,
							char *content, int result)
{
	if (result == 1 && content)
	{
		heredoc->content = content;
		heredoc->pipe_fd = string_to_fd(heredoc->content);
		if (heredoc->pipe_fd == -1)
		{
			free(heredoc->content);
			heredoc->content = NULL;
		}
	}
	else if (result == 0)
	{
		heredoc->content = NULL;
		heredoc->pipe_fd = -1;
		shell->exit_status = 130;
	}
	else
	{
		heredoc->content = NULL;
		heredoc->pipe_fd = -1;
	}
}

static void	process_single_heredoc(t_heredoc *heredoc, t_shell *shell)
{
	char	*content;
	int		result;

	if (heredoc->processed || !heredoc->delimiter)
		return ;
	result = process_heredoc_with_fork(heredoc->delimiter, shell, &content);
	handle_heredoc_result(heredoc, shell, content, result);
	heredoc->processed = 1;
}

static void	process_node_heredocs_with_fork(t_node *node, t_shell *shell)
{
	t_heredoc	*heredoc;

	if (!node || !node->heredocs)
		return ;
	heredoc = node->heredocs;
	while (heredoc)
	{
		process_single_heredoc(heredoc, shell);
		heredoc = heredoc->next;
	}
}

void	process_all_heredocs_with_fork(t_node *node, t_shell *shell)
{
	if (!node)
		return ;
	process_node_heredocs_with_fork(node, shell);
	process_all_heredocs_with_fork(node->left, shell);
	process_all_heredocs_with_fork(node->right, shell);
}
