/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 18:11:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/09 17:43:49 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_t_word_parts(t_token *token)
{
	t_word_part *head;
	t_word_part *current;

	if (!token || !token->parts)  // Ajouter une vérification
	return;

	current = token->parts;
	while (current)
	{
		head = current->next;
		if (current->content)
			free(current->content);
		current->next = NULL;
		free(current);
		current = head;
	}
}

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
		if (current->parts)
			free_t_word_parts(current);
		free(current);
		current = next;
	}
}

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

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char *result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
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