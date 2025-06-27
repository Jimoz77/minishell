/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 12:33:46 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/27 16:32:52 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Renvoie la priorité d'un type de token (plus petit = plus prioritaire)
static int	get_priority(t_token_type type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (2);
	if (type == TOKEN_PIPE)
		return (1);
	return (100);
}

// Traite les opérateurs à profondeur zéro

int	find_lowest_priority(t_token *tokens)
{
	int		pos;
	int		i;
	int		lowest;
	t_token	*tmp;
	int		depth;

	pos = -1;
	i = 0;
	lowest = 0;
	depth = 0;
	tmp = tokens;
	
	while (tmp)
    {
        if (tmp->type == TOKEN_LPAREN)
            depth++;
        else if (tmp->type == TOKEN_RPAREN)
            depth--;
        
        if (depth == 0 && is_operator(tmp->type) && !is_redirection(tmp->type))
        {
            int priority = get_priority(tmp->type);
            // Prendre l'opérateur de priorité la plus faible (valeur numérique la plus haute)
            // En cas d'égalité, prendre le plus à droite
            if (priority >= lowest)
            {
                lowest = priority;
                pos = i;
            }
        }
        tmp = tmp->next;
        i++;
    }
	return (pos);
}

/* int	find_lowest_priority(t_token *tokens)
{
	int		pos;
	int		i;
	int		lowest;
	t_token	*tmp;
	int		depth;

	pos = -1;
	i = 0;
	lowest = 100;
	depth = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_LPAREN)
			depth++;
		else if (tmp->type == TOKEN_RPAREN)
			depth--;
		if (process_operator(tmp, depth, &i, &lowest) != -1)
			pos = process_operator(tmp, depth, &i, &lowest);
		tmp = tmp->next;
		i++;
	}
	return (pos);
} */

// Gère le cas où aucun opérateur n'est trouvé
static t_node	*handle_no_operator(t_token *tokens)
{
	t_token	*tokens_copy;
	t_node	*node;

	tokens_copy = tokens;
	node = parse_command_with_redirections(&tokens_copy);
	return (node);
}

// Construit l'arbre de syntaxe (AST) à partir des tokens
t_node	*parse_ast(t_token *tokens)
{
	t_token	*op;
	int		i;

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_LPAREN)
		return (handle_paren_and_op(tokens));
	i = find_lowest_priority(tokens);
	if (i == -1)
		return (handle_no_operator(tokens));
	op = get_token_at(tokens, i);
	if (!op)
		return (handle_no_operator(tokens));
	return (create_op_node(tokens, op));
}
