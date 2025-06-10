/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 15:21:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 15:19:36 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Convertit un type de token en type de nœud de l'AST
t_node_type	token_to_node_type(t_token_type token_type)
{
	if (token_type == TOKEN_PIPE)
		return (NODE_PIPE);
	else if (token_type == TOKEN_AND)
		return (NODE_AND);
	else if (token_type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_CMD);
}

// Compte le nombre de mots (TOKEN_WORD) consécutifs dans la liste
int	count_words(t_token *tokens)
{
	int		count;
	t_token	*temp;

	count = 0;
	temp = find_command_token(tokens);
	while (temp && temp->type == TOKEN_WORD)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

// Libère un tableau de chaînes partiellement rempli
static void	free_cmd_array(char **cmd, int i)
{
	while (--i >= 0)
		free(cmd[i]);
	free(cmd);
}

// Remplit une case du tableau de commandes avec un mot
static int	fill_one_cmd(char **cmd, t_token *token, int i)
{
	char	*word;

	if (token->parts)
	{
		word = build_unquoted_value(token->parts);
		if (!word)
			return (0);
	}
	else
	{
		if (!token->value)
			return (0);
		word = ft_strdup(token->value);
		if (!word)
			return (0);
	}
	cmd[i] = word;
	return (1);
}

// Crée un tableau de chaînes à partir des temp de type TOKEN_WORD
char	**fill_cmd_array(t_token *temp, int count)
{
	char	**cmd;
	int		i;

	cmd = ft_calloc(count + 1, sizeof(char *));
	if (!cmd)
		return (NULL);
	i = 0;
	while (i < count && temp)
	{
		if (temp->type == TOKEN_WORD)
		{
			if (!fill_one_cmd(cmd, temp, i))
			{
				free_cmd_array(cmd, i);
				return (NULL);
			}
			i++;
		}
		temp = temp->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
