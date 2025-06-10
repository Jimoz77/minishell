/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:11:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 18:05:03 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Libère la mémoire de l'arbre (AST) de façon récursive
void	free_ast(t_node *node)
{
	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->cmd)
		free_array(node->cmd);
	if (node->redirections)
		free_redirections(node->redirections);
	if (node->heredocs)
		free_heredocs(node->heredocs);
	free(node);
}

// Libère la mémoire des redirections
void	free_redirections(t_redir *redirections)
{
	t_redir	*current;
	t_redir	*next;

	current = redirections;
	while (current)
	{
		next = current->next;
		if (current->filename)
			free(current->filename);
		free(current);
		current = next;
	}
}

// Libère la mémoire des heredocs
void	free_heredocs(t_heredoc *heredocs)
{
	t_heredoc	*current;
	t_heredoc	*next;

	current = heredocs;
	while (current)
	{
		next = current->next;
		if (current->delimiter)
			free(current->delimiter);
		if (current->content)
			free(current->content);
		free(current);
		current = next;
	}
}

void	free_token(t_token *token)
{
	t_word_part	*parts;
	t_word_part	*tmp;

	if (!token)
		return ;
	parts = token->parts;
	while (parts)
	{
		tmp = parts->next;
		if (parts->content)
			free(parts->content);
		free(parts);
		parts = tmp;
	}
	if (token->value)
		free(token->value);
	free(token);
}

t_token	*delete_token(t_token *head, t_token *target)
{
	t_token	*current;
	t_token	*prev;

	if (!head || !target)
		return (head);
	if (head == target)
	{
		current = head->next;
		free_token(head);
		return (current);
	}
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
