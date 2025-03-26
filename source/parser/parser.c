/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/26 15:33:34 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Renvoie la priorité d’un type de token (plus petit = plus prioritaire)
static int	get_priority(t_token_type type)
{
	if (type == TOKEN_OR)
		return (1);
	if (type == TOKEN_AND)
		return (2);
	if (type == TOKEN_PIPE)
		return (3);
	if (type == TOKEN_REDIRECT_OUT || type == TOKEN_REDIRECT_IN
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (4);
	return (100);
}

// Renvoie la position du token ayant la plus faible priorité
int	find_lowest_priority(t_token *tokens)
{
	int			pos;
	int			i;
	int			lowest;
	t_token		*tmp;

	pos = -1;
	i = 0;
	lowest = 100;
	tmp = tokens;
	while (tmp)
	{
		if (get_priority(tmp->type) < lowest)
		{
			lowest = get_priority(tmp->type);
			pos = i;
		}
		tmp = tmp->next;
		i++;
	}
	return (pos);
}

// Renvoie le pointeur vers le token à une position donnée
static t_token	*get_token_at(t_token *tokens, int pos)
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

// Construit l’arbre de syntaxe (AST) à partir des tokens
t_node	*parse_ast(t_token *tokens)
{
	t_token	*op;
	t_node	*node;
	t_token	*tmp;
	int		i;

	if (!tokens)
		return (NULL);
	if (!tokens->next)
		return (create_cmd_node(tokens));
	i = find_lowest_priority(tokens);
	op = get_token_at(tokens, i);
	if (!op || i == -1)
		return (create_cmd_node(tokens));
	tmp = tokens;
	while (--i > 0 && tmp)
		tmp = tmp->next;
	if (!tmp || !tmp->next || !tmp->next->next)
		return (NULL);
	op = tmp->next;
	tmp->next = NULL;
	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = token_to_node_type(op->type);
	node->cmd = NULL;
	node->left = parse_ast(tokens);
	node->right = parse_ast(op->next);
	return (node);
}
