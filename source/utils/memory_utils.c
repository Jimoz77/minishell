/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:11:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/15 22:17:57 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Libère la mémoire de la liste chaînée de tokens
void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}

// Libère la mémoire de l'arbre (AST) de façon récursive
void	free_ast(t_node *node)
{
	int	i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->cmd)
	{
		i = 0;
		while (node->cmd[i])
		{
			free(node->cmd[i]);
			i++;
		}
		free(node->cmd);
	}
	free(node);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

void	ft_free_split(char **split_array)
{
	int	i;

	if (!split_array)
		return ;
	i = 0;
	while (split_array[i])
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
	return ;
}

void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	free_token(t_token *token)
{
	t_word_part	*parts;
	t_word_part	*tmp;

	if (!token)
		return ;
	// Libération de la liste parts
	parts = token->parts;
	while (parts)
	{
		tmp = parts->next;
		free(parts->content);
		free(parts);
		parts = tmp;
	}
	// Libération du reste
	free(token->value);
	free(token);
}

t_token	*delete_token(t_token *head, t_token *target)
{
	t_token	*current;
	t_token	*prev;

	if (!head || !target)
		return (head);
	// Cas spécial : suppression de la tête
	if (head == target)
	{
		current = head->next;
		free_token(head);
		return (current);
	}
	// Parcours de la liste
	prev = head;
	current = head->next;
	while (current)
	{
		if (current == target)
		{
			prev->next = current->next;
			free_token(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
	return (head);
}