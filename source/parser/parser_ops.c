/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 14:45:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 15:58:31 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_token_at(t_token *tokens, int pos)
{
	int	i;

	i = 0;
	while (tokens && i < pos)
	{
		tokens = tokens->next;
		i++;
	}
	return (tokens);
}

// Copie le contenu d'une partie de mot
static int	copy_word_content(t_word_part *new_current, t_word_part *current)
{
	if (current->content)
	{
		new_current->content = ft_strdup(current->content);
		if (!new_current->content)
			return (0);
	}
	else
		new_current->content = NULL;
	return (1);
}

// Crée une nouvelle partie de mot
static t_word_part	*create_new_word_part(t_word_part *current)
{
	t_word_part	*new_current;

	new_current = malloc(sizeof(t_word_part));
	if (!new_current)
		return (NULL);
	new_current->type = current->type;
	new_current->next = NULL;
	if (!copy_word_content(new_current, current))
	{
		free(new_current);
		return (NULL);
	}
	return (new_current);
}

// Traite l'ajout d'une nouvelle partie de mot
static void	add_word_part_to_list(t_word_part **new_parts,
								t_word_part **prev, t_word_part *new_current)
{
	if (!*new_parts)
		*new_parts = new_current;
	else
		(*prev)->next = new_current;
	*prev = new_current;
}

t_word_part	*duplicate_word_parts(t_word_part *parts)
{
	t_word_part	*new_parts;
	t_word_part	*current;
	t_word_part	*new_current;
	t_word_part	*prev;

	if (!parts)
		return (NULL);
	new_parts = NULL;
	prev = NULL;
	current = parts;
	while (current)
	{
		new_current = create_new_word_part(current);
		if (!new_current)
		{
			if (new_parts)
				free_word_parts(new_parts);
			return (NULL);
		}
		add_word_part_to_list(&new_parts, &prev, new_current);
		current = current->next;
	}
	return (new_parts);
}
