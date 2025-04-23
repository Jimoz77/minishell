/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:35:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/23 16:30:53 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Crée un nœud de type CMD contenant la commande à droite d'une redirection
t_node	*create_redirect_right(t_token *right_part)
{
	t_node	*right;
	char	**cmd;

	right = malloc(sizeof(t_node));
	if (!right)
		return (NULL);
	right->type = NODE_CMD;
	right->left = NULL;
	right->right = NULL;
	cmd = malloc(sizeof(char *) * 2);
	if (!cmd)
	{
		free(right);
		return (NULL);
	}
	cmd[0] = ft_strdup(right_part->value);
	if (!cmd[0])
	{
		free(cmd);
		free(right);
		return (NULL);
	}
	cmd[1] = NULL;
	right->cmd = cmd;
	return (right);
}

// Configure le nœud de redirection avec ses parties gauche et droite
t_node	*setup_redirect_node(t_node *node, t_token *tokens,
							t_token *right_part)
{
	node->right = create_redirect_right(right_part);
	if (!node->right)
	{
		free(node);
		return (NULL);
	}
	node->left = setup_redirect_left(tokens);
	return (node);
}

// Crée le sous-arbre gauche d’un nœud de redirection
t_node	*setup_redirect_left(t_token *tokens)
{
	if (!tokens || is_redirection(tokens->type))
		return (NULL);
	if (tokens->type == TOKEN_WORD && !tokens->next)
		return (create_cmd_node(tokens));
	else
		return (parse_ast(tokens));
}

// Renvoie le token situé à une position donnée dans la liste
t_token	*get_token_at(t_token *tokens, int pos)
{
	int	i;

	i = 0;
	while (tokens)
	{
		if (i == pos)
			return (tokens);
		tokens = tokens->next;
		i++;
	}
	return (NULL);
}
