/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:00:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/28 13:11:18 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Renvoie le pointeur vers le token à une position donnée
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

// Prépare les parties gauche et droite pour le nœud d'opérateur
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

// Initialise un nœud avec l'opérateur et commence le parsing récursif
t_node	*create_op_node(t_token *tokens, t_token *op)
{
	t_node	*node;
	t_token	*right_part;

	prepare_op_parts(tokens, op, &right_part);
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token_to_node_type(op->type);
	node->cmd = NULL;
	node->left = parse_ast(tokens);
	node->right = parse_ast(right_part);
	return (node);
}

// Traite les expressions entre parenthèses avec opérateurs
t_node	*handle_paren_and_op(t_token *tokens)
{
	t_token	*next_op;
	int		i;
	t_token	*op;

	next_op = tokens;
	while (next_op && next_op->type != TOKEN_RPAREN)
		next_op = next_op->next;
	if (next_op && next_op->next)
	{
		i = find_lowest_priority(tokens);
		if (i != -1)
		{
			op = get_token_at(tokens, i);
			return (create_op_node(tokens, op));
		}
	}
	return (parse_parenthesized_expr(tokens));
}
