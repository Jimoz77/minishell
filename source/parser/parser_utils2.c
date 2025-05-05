/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:30:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 14:19:04 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Fonction pour obtenir un nom de fichier sans quotes pour les redirections
char	*get_unquoted_filename(t_node *node)
{
	if (!node || !node->right || !node->right->cmd || !node->right->cmd[0])
		return (NULL);
	return (ft_strdup(node->right->cmd[0]));
}

// Crée un noeud pour une commande simple
t_node	*create_cmd_node(t_token *tokens)
{
	t_node	*node;
	int		count;

	if (!tokens)
		return (NULL);
	count = count_words(tokens);
	if (count <= 0)
		return (NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->heredoc_index = -1;
	node->left = NULL;
	node->right = NULL;
	node->cmd = fill_cmd_array(tokens, count);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (node);
}
