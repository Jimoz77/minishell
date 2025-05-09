/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 18:54:49 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Exécute un nœud selon son type (commande, redirection, etc.)
static int execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
    t_redirect red;
    int status;

    if (node->type == NODE_CMD)
    {
        // Appliquer les redirections
        init_redirect(&red);
        if (shell->redirections && !apply_all_redirections(shell, &red))
        {
            close_redirect_fds(&red);
            restore_std_fds(&red);
            return (1);
        }
        
        // Exécuter la commande
        status = execute_cmd_node(node, envp);
        
        // Restaurer les descripteurs
        close_redirect_fds(&red);
        restore_std_fds(&red);
        return (status);
    }
    else if (node->type == NODE_PAREN)
    {
        return (execute_paren_node(node, envp, shell));
    }
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
    {
        // Pour les nœuds de redirection, on exécute la partie gauche avec les redirections
        init_redirect(&red);
        if (!apply_all_redirections(shell, &red))
        {
            close_redirect_fds(&red);
            restore_std_fds(&red);
            return (1);
        }
        
        // Si la partie gauche existe, on l'exécute
        if (node->left)
            status = execute_ast(node->left, envp, shell);
        else
            status = 0;  // Pas d'erreur si pas de commande (ex: > file)
        
        close_redirect_fds(&red);
        restore_std_fds(&red);
        return (status);
    }
    return (1);
}

// Fonction principale : exécute l'AST complet
int execute_ast(t_node *node, char ***envp, t_shell *shell)
{
    if (!node)
        return (0);
    
    ft_printf("\n--- STARTING EXECUTION ---\n");
    
    return (execute_node_by_type(node, envp, shell));
}
