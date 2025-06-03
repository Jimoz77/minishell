/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/03 15:47:56 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
