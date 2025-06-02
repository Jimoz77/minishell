/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:10:51 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 18:12:21 by lsadikaj         ###   ########.fr       */
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
