/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 03:46:39 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_token_at(t_token *tokens, int pos)
{
	int	i;

	i = 0;
	while (tokens && i < pos)
	{
		tokens = tokens->next;
		i++;
	}
	return (tokens);
}

t_word_part	*duplicate_word_parts(t_word_part *parts)
{
    t_word_part	*new_parts;
    t_word_part	*current;
    t_word_part	*new_current;
    t_word_part	*prev;

    if (!parts)
        return (NULL);
    
    new_parts = NULL;
    prev = NULL;
    current = parts;
    
    while (current)
    {
        new_current = malloc(sizeof(t_word_part));
        if (!new_current)
        {
            if (new_parts)
                free_word_parts(new_parts);
            return (NULL);
        }
        
        new_current->type = current->type;
        new_current->next = NULL;
        
        if (current->content)
        {
            new_current->content = ft_strdup(current->content);
            if (!new_current->content)
            {
                free(new_current);
                if (new_parts)
                    free_word_parts(new_parts);
                return (NULL);
            }
        }
        else
        {
            new_current->content = NULL;
        }
        
        if (!new_parts)
            new_parts = new_current;
        else
            prev->next = new_current;
        
        prev = new_current;
        current = current->next;
    }
    
    return (new_parts);
}

t_token	*duplicate_tokens(t_token *tokens)
{
    t_token	*new_tokens;
    t_token	*current;
    t_token	*new_current;
    t_token	*prev;

    if (!tokens)
        return (NULL);
    
    new_tokens = NULL;
    prev = NULL;
    current = tokens;
    
    while (current)
    {
        // Allouer le nouveau token
        new_current = malloc(sizeof(t_token));
        if (!new_current)
        {
            if (new_tokens)
                free_tokens(new_tokens);
            return (NULL);
        }
        
        // Dupliquer la valeur
        if (current->value)
        {
            new_current->value = ft_strdup(current->value);
            if (!new_current->value)
            {
                free(new_current);
                if (new_tokens)
                    free_tokens(new_tokens);
                return (NULL);
            }
        }
        else
        {
            new_current->value = NULL;
        }
        
        // Copier le type
        new_current->type = current->type;
        
        // Dupliquer les parts
        new_current->parts = duplicate_word_parts(current->parts);
        
        // Initialiser next
        new_current->next = NULL;
        
        // Chaîner le nouveau token
        if (!new_tokens)
            new_tokens = new_current;
        else
            prev->next = new_current;
        
        prev = new_current;
        current = current->next;
    }
    
    return (new_tokens);
}

t_token	*duplicate_tokens_until_pos(t_token *tokens, int pos)
{
    t_token	*new_tokens;
    t_token	*current;
    t_token	*new_current;
    t_token	*prev;
    int		i;

    if (!tokens || pos <= 0)
        return (NULL);
    
    new_tokens = NULL;
    prev = NULL;
    current = tokens;
    i = 0;
    
    while (current && i < pos)
    {
        // Allouer le nouveau token
        new_current = malloc(sizeof(t_token));
        if (!new_current)
        {
            if (new_tokens)
                free_tokens(new_tokens);
            return (NULL);
        }
        
        // Dupliquer la valeur
        if (current->value)
        {
            new_current->value = ft_strdup(current->value);
            if (!new_current->value)
            {
                free(new_current);
                if (new_tokens)
                    free_tokens(new_tokens);
                return (NULL);
            }
        }
        else
        {
            new_current->value = NULL;
        }
        
        // Copier le type
        new_current->type = current->type;
        
        // Dupliquer les parts
        new_current->parts = duplicate_word_parts(current->parts);
        
        // Initialiser next
        new_current->next = NULL;
        
        // Chaîner le nouveau token
        if (!new_tokens)
            new_tokens = new_current;
        else
            prev->next = new_current;
        
        prev = new_current;
        current = current->next;
        i++;
    }
    
    return (new_tokens);
}

static t_token	*split_at_pos(t_token *tokens, int pos)
{
    t_token	*prev;
    t_token	*op;
    t_token	*next_token;
    int		i;

    if (pos == 0)
        return (tokens);
    prev = tokens;
    i = 0;
    while (i < pos - 1 && prev->next)
    {
        prev = prev->next;
        i++;
    }
    op = prev->next;
    if (op)
    {
        next_token = op;
        prev->next = NULL;
        
        // ✅ Libérer le token opérateur immédiatement
        //free_token(op);
        
        return (next_token);
    }
    return (NULL);
}

t_node	*create_op_node(t_token *tokens, t_token *op)
{
    t_node	*node;
    t_token	*left_tokens;
    t_token	*right_tokens;
    t_token	*op_token;
    int		op_pos;
    (void) op;

    node = ft_calloc(1, sizeof(t_node));
    if (!node)
        return (NULL);
    
    // Trouver la position de l'opérateur
    op_pos = find_lowest_priority(tokens);
    op_token = get_token_at(tokens, op_pos);
    if (!op_token)
    {
        free(node);
        return (NULL);
    }
    
    node->type = token_to_node_type(op_token->type);
    
    // ✅ Dupliquer les tokens de gauche (avant l'opérateur)
    left_tokens = duplicate_tokens_until_pos(tokens, op_pos);
    
    // Séparer à la position de l'opérateur pour obtenir les tokens de droite
    right_tokens = split_at_pos(tokens, op_pos);
    
    // Supprimer et libérer le token opérateur de la liste de droite
    if (right_tokens)
        right_tokens = delete_token(right_tokens, right_tokens);
    
    // Parser les sous-arbres
    node->left = parse_ast(left_tokens);
    node->right = parse_ast(right_tokens);
    if (!node->left || !node->right)
    {
		if (left_tokens)
		free_tokens(left_tokens);
        if (right_tokens)
		free_tokens(right_tokens);
        free_ast(node);
        return (NULL);
    }
	else
	{
		free_tokens(left_tokens);
		free_tokens(right_tokens);
	}
    
    return (node);
}

static t_token	*find_matching_paren(t_token *start)
{
	t_token	*current;
	int		depth;

	current = start->next;
	depth = 1;
	while (current && depth > 0)
	{
		if (current->type == TOKEN_LPAREN)
			depth++;
		else if (current->type == TOKEN_RPAREN)
			depth--;
		if (depth == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

static int	count_tokens_until(t_token *start, t_token *end)
{
    t_token	*current;
    int		count;
    
    if (!start || !end)
        return (0);
    
    current = start;
    count = 0;
    while (current && current != end)
    {
        count++;
        current = current->next;
    }
    return (count);
}

t_node	*handle_paren_and_op(t_token *tokens)
{
    t_token	*end_paren;
    t_token	*inner_start;
    t_token	*inner_tokens;
    t_node	*inner_node;
    int		inner_count;

    if (!tokens || tokens->type != TOKEN_LPAREN)
        return (NULL);
    
    end_paren = find_matching_paren(tokens);
    if (!end_paren)
        return (NULL);
    
    inner_start = tokens->next;
    if (inner_start == end_paren)
        return (NULL);
    
    // ✅ Compter les tokens intérieurs
    inner_count = count_tokens_until(inner_start, end_paren);
    
    // ✅ Dupliquer les tokens intérieurs indépendamment
    inner_tokens = duplicate_tokens_until_pos(inner_start, inner_count);
    if (!inner_tokens)
        return (NULL);
    
    // Parser le sous-arbre avec les tokens dupliqués
    inner_node = parse_ast(inner_tokens);
    
    // ✅ Libérer les tokens dupliqués après parsing
    free_tokens(inner_tokens);
    
    if (!inner_node)
        return (NULL);
    
    // Gérer les opérateurs après les parenthèses
    if (end_paren->next && is_operator(end_paren->next->type))
        return (handle_op_after_paren(inner_node, tokens, end_paren->next));
    
    return (inner_node);
}
