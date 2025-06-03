/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:30:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/03 17:49:46 by lsadikaj         ###   ########.fr       */
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
			if (current->type == TOKEN_HEREDOC)
			{
				add_heredoc(cmd_node, current->next->value);
				add_redirection(cmd_node, current->type, 
					current->next->value);
			}
			else
				add_redirection(cmd_node, current->type, 
					current->next->value);
			// Supprimer le token de redirection et le filename
			if (prev)
				prev->next = current->next->next;
			else
				*tokens = current->next->next;
			free(current->value);
			free(current->next->value);
			free(current->next);
			free(current);
			current = (prev) ? prev->next : *tokens;
		}
		else
		{
			prev = current;
			current = next;
		}
	}
	return (cmd_node);
}

t_node *parse_command_with_redirections(t_token **tokens)
{
    t_node *cmd_node;

    if (!tokens || !*tokens)
        return (NULL);
    
    // Créer le nœud de commande avec tous les tokens
    cmd_node = create_cmd_node(*tokens);
    if (!cmd_node)
        return (NULL);
    
    // Extraire et attacher les redirections au nœud
    extract_redirections(tokens, cmd_node);
    
    // Reconstruire le tableau cmd sans les redirections
    
    
    return (cmd_node);
}
