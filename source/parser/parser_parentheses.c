/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parentheses.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:40:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 14:54:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Trouve la parenthèse fermante correspondante
t_token	*find_matching_paren(t_token *start)
{
	int		depth;
	t_token	*current;

	if (!start || start->type != TOKEN_LPAREN)
		return (NULL);
	depth = 1;
	current = start->next;
	while (current && depth > 0)
	{
		if (current->type == TOKEN_LPAREN)
			depth++;
		else if (current->type == TOKEN_RPAREN)
			depth--;
		if (depth > 0)
			current = current->next;
	}
	return (current);
}

// Crée un nouveau token copié depuis un original
static t_token	*create_token_copy(t_token *current)
{
	t_token		*new;
	t_word_part	*part_current;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->value = ft_strdup(current->value);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->type = current->type;
	new->next = NULL;
	new->parts = NULL;
	part_current = current->parts;
	while (part_current)
	{
		add_word_part(&new->parts, part_current->content,
			part_current->type);
		part_current = part_current->next;
	}
	return (new);
}

// Clone une liste de tokens entre start et end (non inclus)
t_token	*clone_tokens(t_token *start, t_token *end)
{
	t_token		*result;
	t_token		*current;
	t_token		*new;
	t_token		*tmp;

	result = NULL;
	current = start;
	while (current && current != end)
	{
		new = create_token_copy(current);
		if (!new)
		{
			free_tokens(result);
			return (NULL);
		}
		if (!result)
			result = new;
		else
		{
			tmp = result;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = new;
		}
		current = current->next;
	}
	return (result);
}

// Vérifie les doubles parenthèses et gère les cas spéciaux
int	handle_arithmetic_expr(t_token *tokens)
{
	if (tokens && tokens->type == TOKEN_LPAREN
		&& tokens->next && tokens->next->type == TOKEN_LPAREN)
	{
		ft_printf("minishell: syntax error near unexpected token `('\n");
		return (1);
	}
	return (0);
}

// Vérifie et traite les opérateurs après une parenthèse fermante
t_node	*check_paren_operators(t_node *node, t_token *closing)
{
	t_token	*next_token;
	t_node	*op_node;

	if (!closing || !closing->next)
		return (node);
	next_token = closing->next;
	if (is_redirection(next_token->type)
		&& next_token->next && next_token->next->type == TOKEN_WORD)
	{
		op_node = malloc(sizeof(t_node));
		if (!op_node)
			return (node);
		op_node->type = token_to_node_type(next_token->type);
		op_node->cmd = NULL;
		op_node->left = node;
		op_node->right = create_redirect_right(next_token->next);
		if (!op_node->right)
		{
			free(op_node);
			return (node);
		}
		if (next_token->next && next_token->next->next
			&& is_redirection(next_token->next->next->type))
			return (check_paren_operators(op_node, next_token->next));
		return (op_node);
	}
	return (node);
}
