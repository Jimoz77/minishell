/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 17:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/08 19:28:35 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*find_token_before(t_token *start, t_token *target)
{
	t_token	*current;
	t_token	*prev;

	if (!start || !target || start == target)
		return (NULL);
	current = start;
	prev = NULL;
	while (current && current != target)
	{
		prev = current;
		current = current->next;
	}
	return (prev);
}

t_node	*handle_op_after_paren(t_node *inner_node, t_token *inner_start, t_token *op_token)
{
    t_node	*current_node;
    t_token	*current_token;
    (void) inner_start;

    if (!inner_node || !op_token)
        return (inner_node);

    current_node = inner_node;
    current_token = op_token;

    // Traiter toutes les redirections en chaîne
    while (current_token && is_redirection(current_token->type))
    {
        if (current_token->next && current_token->next->value)
        {
            // Ajouter la redirection au nœud courant
            add_redirection(current_node, current_token->type, current_token->next->value);
            
            // Passer au token suivant (filename) puis au suivant (potentielle autre redirection)
            current_token = current_token->next;
            if (current_token)
                current_token = current_token->next;
        }
        else
        {
            break;
        }
    }

    // Si il reste des tokens après les redirections, traiter les autres opérateurs
    if (current_token && !is_redirection(current_token->type))
    {
        t_node *op_node = ft_calloc(1, sizeof(t_node));
        if (!op_node)
        {
            free_ast(current_node);
            return (NULL);
        }
        
        op_node->type = token_to_node_type(current_token->type);
        op_node->left = current_node;
        
        if (current_token->next)
            op_node->right = parse_ast(current_token->next);
        
        return (op_node);
    }

    return (current_node);
}
