/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 23:08:42 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	exec_with_redirection(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;
	int			status;

	init_redirect(&red);
	// Maintenant on applique les redirections depuis le nœud
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		if (errno == ENOENT)
			return (127);
		else if (errno == EACCES)
			return (126);
		else
			return (1);
	}
	status = 0;
	if (node->left)
		status = execute_ast(node->left, envp, shell);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (status);
}

int	execute_and_node(t_node *node, char ***envp, t_shell *shell)
{
	int	left_status;

	if (!node || !node->left)
		return (1);
	left_status = execute_node_by_type(node->left, envp, shell);
	if (left_status == 0 && node->right)
		return (execute_node_by_type(node->right, envp, shell));
	return (left_status);
}

int	execute_or_node(t_node *node, char ***envp, t_shell *shell)
{
	int	left_status;

	if (!node || !node->left)
		return (1);
	left_status = execute_node_by_type(node->left, envp, shell);
	if (left_status != 0 && node->right)
		return (execute_node_by_type(node->right, envp, shell));
	return (left_status);
}

int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
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
	else if (is_redirect_node(node->type))
		return (exec_with_redirection(node, envp, shell));
	return (1);
}

int	execute_ast(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	// D'abord, traiter tous les heredocs de l'AST
	process_all_heredocs(node);
	// Ensuite exécuter l'AST
	return (execute_node_by_type(node, envp, shell));
}
