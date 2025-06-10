/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:10:51 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 15:19:47 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_inside_parentheses(t_token *tokens, t_token *target)
{
	t_token	*current;
	int		depth;

	current = tokens;
	depth = 0;
	while (current && current != target)
	{
		if (current->type == TOKEN_LPAREN)
			depth++;
		else if (current->type == TOKEN_RPAREN)
			depth--;
		current = current->next;
	}
	return (depth > 0);
}

t_token	*skip_parentheses_block(t_token *start)
{
	t_token	*current;
	int		depth;

	if (!start || start->type != TOKEN_LPAREN)
		return (start);
	current = start->next;
	depth = 1;
	while (current && depth > 0)
	{
		if (current->type == TOKEN_LPAREN)
			depth++;
		else if (current->type == TOKEN_RPAREN)
			depth--;
		if (depth == 0)
			return (current->next);
		current = current->next;
	}
	return (NULL);
}

// Ajoute un heredoc à la liste chaînée

void	add_heredoc(t_node *node, char *delimiter)
{
	t_heredoc	*new_heredoc;
	t_heredoc	*current;

	if (!node || !delimiter)
		return ;
	new_heredoc = malloc(sizeof(t_heredoc));
	if (!new_heredoc)
		return ;
	new_heredoc->delimiter = ft_strdup(delimiter);
	new_heredoc->content = NULL;
	new_heredoc->processed = 0;
	new_heredoc->pipe_fd = -1;
	new_heredoc->next = NULL;
	if (!node->heredocs)
		node->heredocs = new_heredoc;
	else
	{
		current = node->heredocs;
		while (current->next)
			current = current->next;
		current->next = new_heredoc;
	}
}
