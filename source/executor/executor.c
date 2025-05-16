/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/16 17:30:44 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Exécute les redirections et la partie gauche si elle existe
static int	exec_with_redirection(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;
	int			status;

	init_redirect(&red);
	if (!apply_all_redirections(shell, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	status = 0;
	if (node->left)
		status = execute_ast(node->left, envp, shell);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (status);
}
int	execute_and_node(t_node *node, char ***envp)
{
	int	left_status;

	left_status = execute_node_by_type(node->left, envp);
	if (left_status == 0)
	{
		if (node->right)
			return (execute_node_by_type(node->right, envp));
	}
	return (left_status);
}

int	execute_or_node(t_node *node, char ***envp)
{
	int	left_status;

	left_status = execute_node_by_type(node->left, envp);
	if (left_status != 0)
	{
		if (node->right)
			return (execute_node_by_type(node->right, envp));
	}
	return (left_status);
}
// Exécute un nœud selon son type (commande, redirection, etc.)
static int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	if (node->type == NODE_CMD)
		return (execute_cmd_node(node, envp));
	else if (node->type == NODE_PAREN)
		return (execute_paren_node(node, envp, shell));
	else if (node->type == NODE_PIPE)
		return (execute_pipe_node);
	else if (node->type == NODE_AND)
		return (execute_and_node);
	else if (node->type == NODE_OR)
		return (execute_or_node);
	else if (is_redirect_node(node->type))
		return (exec_with_redirection(node, envp, shell));
	return (1);
}

// Fonction principale : exécute l'AST complet
int	execute_ast(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	return (execute_node_by_type(node, envp, shell));
}