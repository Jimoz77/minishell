/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:10:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 18:22:56 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_redirect(t_redirect *red)
{
	red->stdin_fd = -1;
	red->stdout_fd = -1;
	red->saved_stdin = dup(STDIN_FILENO);
	red->saved_stdout = dup(STDOUT_FILENO);
}

void	close_redirect_fds(t_redirect *red)
{
	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
	if (red->stdout_fd != -1)
	{
		close(red->stdout_fd);
		red->stdout_fd = -1;
	}
}

void	restore_std_fds(t_redirect *red)
{
	if (red->saved_stdin != -1)
	{
		dup2(red->saved_stdin, STDIN_FILENO);
		close(red->saved_stdin);
		red->saved_stdin = -1;
	}
	if (red->saved_stdout != -1)
	{
		dup2(red->saved_stdout, STDOUT_FILENO);
		close(red->saved_stdout);
		red->saved_stdout = -1;
	}
}

int	is_redirect_node(t_node_type type)
{
	return (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT
		|| type == NODE_APPEND || type == NODE_HEREDOC);
}

t_node	*find_command_node(t_node *node)
{
	t_node	*result;

	if (!node)
		return (NULL);
	if (node->type == NODE_CMD)
		return (node);
	result = find_command_node(node->left);
	if (result)
		return (result);
	return (find_command_node(node->right));
}
