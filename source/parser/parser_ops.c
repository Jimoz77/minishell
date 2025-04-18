/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:03:25 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/16 12:38:17 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

// Coupe la liste de tokens en deux autour du token opérateur
static void	prepare_op_parts(t_token *tokens, t_token *op,
							t_token **right_part)
{
	t_token	*tmp;

	*right_part = op->next;
	tmp = tokens;
	while (tmp && tmp->next != op)
		tmp = tmp->next;
	if (tmp)
		tmp->next = NULL;
}

// Initialise un nœud opérateur avec ses branches gauche et droite
static t_node	*setup_operator_node(t_node *node, t_token *tokens,
									t_token *right_part)
{
	node->left = parse_ast(tokens);
	node->right = parse_ast(right_part);
	return (node);
}

// Initialise un nœud de redirection complet
t_node	*init_redirect_node(t_token *tokens, t_token *right_part,
							t_token_type type)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token_to_node_type(type);
	node->cmd = NULL;
	node->right = create_redirect_right(right_part);
	if (!node->right)
	{
		free(node);
		return (NULL);
	}
	if (!tokens)
		node->left = NULL;
	else
		node->left = setup_redirect_left(tokens);
	return (node);
}

// Crée un nœud opérateur ou redirection à partir d’un token opérateur
t_node	*create_op_node(t_token *tokens, t_token *op)
{
	t_token	*right_part;

	prepare_op_parts(tokens, op, &right_part);
	if (is_redirection(op->type) && right_part
		&& right_part->type == TOKEN_WORD)
		return (init_redirect_node(tokens, right_part, op->type));
	t_node *node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token_to_node_type(op->type);
	node->cmd = NULL;
	return (setup_operator_node(node, tokens, right_part));
}
