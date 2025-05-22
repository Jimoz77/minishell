/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_collect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:05:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/22 12:51:37 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Crée un nouvel élément heredoc
static t_heredoc	*create_heredoc(char *delimiter, char *content)
{
	t_heredoc	*new;

	new = malloc(sizeof(t_heredoc));
	if (!new)
		return (NULL);
	new->delimiter = ft_strdup(delimiter);
	if (!new->delimiter)
	{
		free(new);
		return (NULL);
	}
	if (content)
		new->content = ft_strdup(content);
	else
		new->content = NULL;
	new->processed = 0;
	new->pipe_fd = -1;
	new->next = NULL;
	return (new);
}

// Ajoute un heredoc à la liste chaînée
void	add_heredoc(t_heredoc **list, char *delimiter, char *content)
{
	t_heredoc	*new;
	t_heredoc	*tmp;

	new = create_heredoc(delimiter, content);
	if (!new)
		return ;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Ajoute une redirection à la liste chaînée
void	add_redirection(t_redir **list, t_token_type type, char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->filename = ft_strdup(filename);
	if (!new->filename)
	{
		free(new);
		return ;
	}
	new->next = NULL;
	if (!*list)
		*list = new;
	else
	{
		tmp = *list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

// Compte le niveau de parenthèses ouvertes
static int	count_paren_level(t_token *tokens, t_token *target)
{
	t_token	*current;
	int		level;

	current = tokens;
	level = 0;
	while (current && current != target)
	{
		if (current->type == TOKEN_LPAREN)
			level++;
		else if (current->type == TOKEN_RPAREN)
			level--;
		current = current->next;
	}
	return (level);
}

// Collecte les redirections seulement si elles sont en dehors des parenthèses
void	collect_redirections(t_token *tokens, t_shell *shell)
{
	t_token	*current;
	t_token	*next;

	shell->redirections = NULL;
	shell->heredocs = NULL;
	current = tokens;
	while (current)
	{
		if (is_redirection(current->type) && current->next
			&& current->next->type == TOKEN_WORD)
		{
			// Vérifier si la redirection est en dehors des parenthèses
			if (count_paren_level(tokens, current) == 0)
			{
				next = current->next;
				if (current->type == TOKEN_HEREDOC)
					add_heredoc(&shell->heredocs, next->value, NULL);
				add_redirection(&shell->redirections, current->type, next->value);
			}
		}
		current = current->next;
	}
}

// Nouvelle fonction pour collecter les redirections à l'intérieur des parenthèses
void	collect_redirections_in_parens(t_token *tokens, t_shell *shell)
{
	t_token	*current;
	t_token	*next;

	if (!shell->redirections)
		shell->redirections = NULL;
	if (!shell->heredocs)
		shell->heredocs = NULL;
	
	current = tokens;
	while (current)
	{
		if (is_redirection(current->type) && current->next
			&& current->next->type == TOKEN_WORD)
		{
			next = current->next;
			if (current->type == TOKEN_HEREDOC)
				add_heredoc(&shell->heredocs, next->value, NULL);
			add_redirection(&shell->redirections, current->type, next->value);
		}
		current = current->next;
	}
}
