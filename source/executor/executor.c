/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 13:54:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 19:41:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si le type correspond à une redirection
int	is_redirect_node(t_node_type type)
{
	return (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT
		|| type == NODE_APPEND || type == NODE_HEREDOC);
}

/* Collecte tous les heredocs depuis l'AST et les stocke */
int	process_all_heredocs(t_node *root, t_shell *shell)
{
	t_node	*heredocs[100];
	int		count;
	int		i;

	count = 0;
	collect_all_heredocs(root, heredocs, &count);
	if (count == 0)
		return (0);
	shell->heredocs = ft_calloc(count + 1, sizeof(char *));
	if (!shell->heredocs)
	{
		perror("minishell: heredoc alloc");
		return (1);
	}
	i = 0;
	while (i < count)
	{
		if (process_single_heredoc(heredocs[i], i, shell) == -1)
			return (1);
		heredocs[i]->heredoc_index = i;
		i++;
	}
	return (0);
}

// Exécute un nœud selon son type (commande, redirection, etc.)
int	execute_node_by_type(t_node *node, char ***envp, t_shell *shell)
{
	if (node->type == NODE_CMD)
	{
		if (node->heredoc_index >= 0)
			return (execute_combined_node(node, *envp, shell));
		return (execute_cmd_node(node, envp));
	}
	if (is_redirect_node(node->type))
		return (execute_combined_node(node, *envp, shell));
	if (node->type == NODE_PAREN)
		return (execute_paren_node(node, envp, shell));
	if (node->type == NODE_PIPE || node->type == NODE_AND
		|| node->type == NODE_OR)
	{
		ft_printf("minishell: type de noeud non supporté\n");
		return (1);
	}
	return (1);
}

// Fonction principale : exécute l'AST complet
int	execute_ast(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	if (process_all_heredocs(node, shell) != 0)
		return (1);
	return (execute_node_by_type(node, envp, shell));
}
