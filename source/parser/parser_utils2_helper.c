/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 15:40:06 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 16:34:17 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Met à jour les pointeurs après traitement d'une redirection
static void	update_after_redirect(t_token **current, t_token **prev,
								t_token **tokens)
{
	if (*prev)
		*current = (*prev)->next;
	else
		*current = *tokens;
}

// Met à jour les pointeurs pour un token normal
static void	update_normal_token(t_token **current, t_token **prev,
								t_token *next)
{
	*prev = *current;
	*current = next;
}

// Traite une redirection et met à jour les pointeurs
static void	process_redirection(t_token *current, t_node *cmd_node,
								t_token **prev, t_token **tokens)
{
	if (current->type == TOKEN_HEREDOC)
	{
		add_heredoc(cmd_node, current->next->value);
		add_redirection(cmd_node, current->type, current->next->value);
	}
	else
		add_redirection(cmd_node, current->type, current->next->value);
	if (*prev)
		delete_token(current, current->next);
	else
		*tokens = current->next->next;
}

static t_node	*extract_redirections(t_token **tokens, t_node *cmd_node)
{
	t_token	*current;
	t_token	*prev;
	t_token	*next;

	if (!tokens || !*tokens || !cmd_node)
		return (cmd_node);
	current = *tokens;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (is_redirection(current->type) && current->next
			&& current->next->type == TOKEN_WORD)
		{
			process_redirection(current, cmd_node, &prev, tokens);
			update_after_redirect(&current, &prev, tokens);
		}
		else
			update_normal_token(&current, &prev, next);
	}
	return (cmd_node);
}

t_node	*parse_command_with_redirections(t_token **tokens)
{
	t_node	*cmd_node;

	if (!tokens || !*tokens)
		return (NULL);
	cmd_node = create_cmd_node(*tokens);
	if (!cmd_node)
		return (NULL);
	extract_redirections(tokens, cmd_node);
	return (cmd_node);
}
