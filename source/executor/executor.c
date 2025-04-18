/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/17 14:54:31 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Return 1 si c'est une redirection
int	is_redirect_node(t_node_type type)
{
	return (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT ||
			type == NODE_APPEND || type == NODE_HEREDOC);
}

// Vérifie si le nœud ou un de ses ancêtres est une redirection
static int has_multiple_redirects(t_node *node) 
{
    if (!node)
        return (0);
    // Si ce nœud est une redirection
    if (is_redirect_node(node->type))
	{
		// Vérifier si le noeud gauche est aussi une redirection
		if (node->left && is_redirect_node(node->left->type))
			return (1);
		// Si le noeud gauche a des redirections, cette branche en a plusieurs
		if (node->left && has_multiple_redirects(node->left))
			return (1);
	}
    // Vérifier récursivement les enfants si ce n'est pas une redirection
    return (has_multiple_redirects(node->left) ||
        has_multiple_redirects(node->right));
}

// Execute le noeud selon le type
static int	execute_node_by_type(t_node *node, char **envp)
{
	static int (*execute[9])(t_node *, char **) = {
	[NODE_CMD] = execute_cmd_node,
	[NODE_REDIRECT_IN] = execute_redirect_node,
	[NODE_REDIRECT_OUT] = execute_redirect_node,
	[NODE_APPEND] = execute_redirect_node,
	[NODE_HEREDOC] = handle_heredoc,
	[NODE_PAREN] = execute_paren_node,
	};
	if (node->type == NODE_PIPE || node->type == NODE_AND
		|| node->type == NODE_OR)
	{
		ft_printf("minishell: type de noeud non supporté\n");
		return (1);
	}
	return(execute[node->type](node, envp));
}

// Fonction principale d'execution
int execute_ast(t_node *node, char **envp)
{
    int status;
	int	multiple;

    status = 0;
    if (!node)
        return (0);
    multiple = has_multiple_redirects(node);
    if (multiple)
        return (execute_combined_node(node, envp));
    status = execute_node_by_type(node, envp);
    return (status);
}
