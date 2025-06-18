/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:30:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/18 13:45:25 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Fonction pour obtenir un nom de fichier sans quotes pour les redirections
t_token	*find_command_token(t_token *tokens)
{
	if (!tokens)
		return (NULL);
	while (tokens)
	{
		if (is_redirection(tokens->type) && tokens->next)
		{
			if (tokens->next->next)
				tokens = tokens->next->next;
			else
				tokens = tokens->next;
		}
		if (tokens->type == TOKEN_WORD)
			return (tokens);
	}
	return (NULL);
}

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
	t_token	*temp;

	if (!tokens)
		return (NULL);
	count = count_words(tokens);
	if (count <= 0 && tokens->type == TOKEN_WORD)
		return (NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->heredocs = NULL;
	node->redirections = NULL;
	temp = find_command_token(tokens);
	node->cmd = fill_cmd_array(temp, count);
	return (node);
}

void	add_redirection(t_node *node, t_token_type type, char *filename)
{
	t_redir	*new_redir;
	t_redir	*current;

	if (!node || !filename)
		return ;
	new_redir = malloc(sizeof(t_redir));
	if (!new_redir)
		return ;
	new_redir->type = type;
	new_redir->filename = ft_strdup(filename);
	new_redir->next = NULL;
	if (!node->redirections)
		node->redirections = new_redir;
	else
	{
		current = node->redirections;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
}
