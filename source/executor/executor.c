/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 15:00:09 by lsadikaj         ###   ########.fr       */
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

// Exécute un nœud selon son type (commande, redirection, etc.)
static int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	if (node->type == NODE_CMD)
		return (execute_cmd_node(node, envp));
	else if (node->type == NODE_PAREN)
		return (execute_paren_node(node, envp, shell));
	else if (node->type == NODE_PIPE)
	{
		ft_printf("minishell: opérateur pipe '|' non pris en charge\n");
		return (1);
	}
	else if (node->type == NODE_AND)
	{
		ft_printf("minishell: opérateur logique '&&' non pris en charge\n");
		return (1);
	}
	else if (node->type == NODE_OR)
	{
		ft_printf("minishell: opérateur logique '||' non pris en charge\n");
		return (1);
	}
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
