/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/09 13:18:33 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void debug_ast(t_node *node, int level)
{
    if (!node) return;
    
    for (int i = 0; i < level; i++) printf("  ");
    
    printf("Type: %d", node->type);
    if (node->redirections)
        printf(" [HAS REDIRECTIONS]");
    if (node->cmd && node->cmd[0])
        printf(" CMD: %s", node->cmd[0]);
    printf("\n");
    
    if (node->left) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("LEFT:\n");
        debug_ast(node->left, level + 1);
    }
    if (node->right) {
        for (int i = 0; i < level; i++) printf("  ");
        printf("RIGHT:\n");
        debug_ast(node->right, level + 1);
    }
}

int execute_and_node(t_node *node, char ***envp, t_shell *shell)
{
    int left_status;

    if (!node || !node->left)
        return (1);
    
    // Exécuter les commandes (les redirections sont gérées par execute_node_by_type)
    left_status = execute_ast(node->left, envp, shell);
    if (left_status == 0 && node->right)
        left_status = execute_ast(node->right, envp, shell);
    
    return (left_status);
}

int execute_or_node(t_node *node, char ***envp, t_shell *shell)
{
    int left_status;

    if (!node || !node->left)
        return (1);
    
    // Exécuter les commandes (les redirections sont gérées par execute_node_by_type)
    left_status = execute_ast(node->left, envp, shell);
    if (left_status != 0 && node->right)
        left_status = execute_ast(node->right, envp, shell);
    
    return (left_status);
}

int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect red;
    int use_redirection = 0;
    int result;

    if (!node)
        return (0);
    
    // Gérer les redirections globalement pour tous les types de nœuds
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
 // Exécuter le nœud selon son type
	if (node->type == NODE_CMD)
		result = execute_cmd_node(node, envp, shell);
	else if (node->type == NODE_PAREN)
		result = execute_paren_node(node, envp, shell);
	else if (node->type == NODE_PIPE)
		result = execute_pipe_node(node, envp, shell);
	else if (node->type == NODE_AND)
		result = execute_and_node(node, envp, shell);
	else if (node->type == NODE_OR)
		result = execute_or_node(node, envp, shell);
	else
		result = 1;
	// Restaurer les descripteurs si on a appliqué des redirections
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

	   // DEBUG: Afficher l'AST
/* 	printf("=== AST DEBUG ===\n");
	debug_ast(node, 0);
	printf("================\n"); */
	// D'abord, traiter tous les heredocs de l'AST
	process_all_heredocs(node);
	return (execute_node_by_type(node, envp, shell));
}