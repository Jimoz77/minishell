/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:30:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/02 17:55:19 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Fonction pour obtenir un nom de fichier sans quotes pour les redirections
char	*get_unquoted_filename(t_token *tokens)
{
	char	*filename;

	if (!tokens || tokens->type != TOKEN_WORD)
		return (NULL);
	if (tokens->parts)
		filename = build_unquoted_value(tokens->parts);
	else
		filename = ft_strdup(tokens->value);
	return (filename);
}

// Crée un noeud pour une commande simple
t_node	*create_cmd_node(t_token *tokens)
{
	t_node	*node;
	int		count;

	if (!tokens)
		return (NULL);
	count = count_words(tokens);
	if (count <= 0)
		return (NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->heredocs = NULL;
	node->redirections = NULL;
	node->cmd = fill_cmd_array(tokens, count);
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (node);
}

static void	attach_redirections(t_node *node, t_token **tokens)
{
	t_token	*current;
	char	*filename;
	char	*delimiter;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type) && current->next
			&& current->next->type == TOKEN_WORD)
		{
			if (current->type == TOKEN_HEREDOC)
			{
				delimiter = get_unquoted_filename(current->next);
				if (delimiter)
					add_heredoc(&node->heredocs, delimiter, NULL);
			}
			else
			{
				filename = get_unquoted_filename(current->next);
				if (filename)
					add_redirections(&node->redirections, current->type,
										filename);
			}
			current = current->next;
		}
		else if (current->type == TOKEN_WORD)
			current = current->next;
		else
			break ;
	}
	*tokens = current;
}

t_node	*parse_command_with_redirections(t_token **tokens)
{
	t_node	*node;
	t_token	*cmd_tokens;

	if (!tokens || !*tokens)
		return (NULL);
	cmd_tokens = *tokens;
	node = create_cmd_node(cmd_tokens);
	if (!node)
		return (NULL);
	attach_redirections_to_node(node, tokens);
	return (node);
}
