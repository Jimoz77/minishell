/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/19 19:12:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_by_node_type(t_node *node, char ***envp, t_shell *shell)
{
	if (node->type == NODE_CMD)
		return (execute_cmd_node(node, envp, shell));
	else if (node->type == NODE_PAREN)
		return (execute_paren_node(node, envp, shell));
	else if (node->type == NODE_PIPE)
		return (execute_pipe_node(node, envp, shell));
	else if (node->type == NODE_AND)
		return (execute_and_node(node, envp, shell));
	else if (node->type == NODE_OR)
		return (execute_or_node(node, envp, shell));
	else
		return (1);
}

int	execute_and_node(t_node *node, char ***envp, t_shell *shell)
{
	int	left_status;

	if (!node || !node->left)
		return (1);
	left_status = execute_ast(node->left, envp, shell);
	if (left_status == 0 && node->right)
		left_status = execute_ast(node->right, envp, shell);
	return (left_status);
}

int	execute_or_node(t_node *node, char ***envp, t_shell *shell)
{
	int	left_status;

	if (!node || !node->left)
		return (1);
	left_status = execute_ast(node->left, envp, shell);
	if (left_status != 0 && node->right)
		left_status = execute_ast(node->right, envp, shell);
	return (left_status);
}

int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;
	int			use_redirection;
	int			result;

	if (!node)
		return (0);
	use_redirection = 0;
	if (node->redirections)
	{
		init_redirect(&red);
		if (!apply_node_redirections(node, &red))
		{
			close_redirect_fds(&red);
			restore_std_fds(&red);
			return (1);
		}
		use_redirection = 1;
	}
	result = execute_by_node_type(node, envp, shell);
	if (use_redirection)
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
	}
	return (result);
}

int	execute_ast(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	process_all_heredocs(node);
	return (execute_node_by_type(node, envp, shell));
}
