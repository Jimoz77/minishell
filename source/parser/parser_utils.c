/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:21:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/26 15:33:40 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Traduit un token type en node type pour l’AST
t_node_type	token_to_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	else if (token_type == TOKEN_AND)
		return (NODE_AND);
	else if (token_type == TOKEN_OR)
		return (NODE_OR);
	else if (token_type == TOKEN_REDIRECT_OUT)
		return (NODE_REDIRECT_OUT);
	else if (token_type == TOKEN_REDIRECT_IN)
		return (NODE_REDIRECT_IN);
	else if (token_type == TOKEN_APPEND)
		return (NODE_APPEND);
	else if (token_type == TOKEN_HEREDOC)
		return (NODE_HEREDOC);
	return (NODE_CMD);
}

// Compte le nombre de TOKEN_WORD dans la liste
int	count_words(t_token *tokens)
{
	int	count = 0;

	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

// Alloue et remplit le tableau de cmd[] avec les valeurs des tokens
char	**fill_cmd_array(t_token *tokens, int count)
{
	char	**cmd;
	int		i;

	cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd)
		return (NULL);
	i = 0;
	while (i < count && tokens && tokens->type == TOKEN_WORD)
	{
		if (!tokens->value)
		{
			while (--i >= 0)
				free(cmd[i]);
			free(cmd);
			return (NULL);
		}
		cmd[i] = ft_strdup(tokens->value);
		if (!cmd[i])
		{
			while (--i >= 0)
				free(cmd[i]);
			free(cmd);
			return (NULL);
		}
		tokens = tokens->next;
		i++;
	}
	cmd[i] = NULL;
	return (cmd);
}

// Crée un nœud de type NODE_CMD à partir des tokens
t_node	*create_cmd_node(t_token *tokens)
{
	t_node	*node;
	int		count;
	t_token	*tmp;

	if (!tokens)
		return (NULL);
	tmp = tokens;
	while (tmp)
		tmp = tmp->next;
	count = count_words(tokens);
	if (count <= 0)
		return (NULL);
	node = ft_calloc(1, sizeof(t_node));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->left = NULL;
	node->right = NULL;
	node->cmd = fill_cmd_array(tokens, count);
	if (!node->cmd)
		return (free(node), NULL);
	return (node);
}
