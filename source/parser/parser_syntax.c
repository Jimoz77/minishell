/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 17:51:30 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 15:48:29 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR
		|| type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || type == TOKEN_REDIRECT_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

int	check_paren_content(t_token *start, t_token *end)
{
	t_token	*current;
	int		has_content;

	if (!start || !end || start == end)
		return (0);
	current = start;
	has_content = 0;
	while (current && current != end)
	{
		if (current->type == TOKEN_WORD || is_operator(current->type))
			has_content = 1;
		current = current->next;
	}
	return (has_content);
}
