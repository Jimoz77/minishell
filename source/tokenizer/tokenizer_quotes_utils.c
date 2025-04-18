/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quotes_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:40:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/14 16:35:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ajoute une partie de mot à la liste des parties
void	add_word_part(t_word_part **parts, char *content, t_quote_type type)
{
	t_word_part	*new_part;
	t_word_part	*tmp;

	if (!content || !*content)
		return ;
	new_part = malloc(sizeof(t_word_part));
	if (!new_part)
		return ;
	new_part->content = ft_strdup(content);
	if (!new_part->content)
	{
		free(new_part);
		return ;
	}
	new_part->type = type;
	new_part->next = NULL;
	if (!*parts)
		*parts = new_part;
	else
	{
		tmp = *parts;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_part;
	}
}

// Libère les ressources d'une liste de parties de mots
void	free_word_parts(t_word_part *parts)
{
	t_word_part	*tmp;

	while (parts)
	{
		tmp = parts;
		parts = parts->next;
		if (tmp->content)
			free(tmp->content);
		free(tmp);
	}
}

// Parse une partie de mot entre guillemets
int	parse_quoted_part(char *input, int *i, t_word_part **parts,
							t_quote_type type)
{
	int		start;
	char	quote;
	char	*content;

	start = *i;
	if (type == QUOTE_SINGLE)
		quote = '\'';
	else if (type == QUOTE_DOUBLE)
		quote = '"';
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (!input[*i])
	{
		ft_printf("minishell: syntax error: unclosed quote\n");
		return (-1);
	}
	content = ft_substr(input, start + 1, *i - start - 1);
	if (!content)
		return (-1);
	add_word_part(parts, content, type);
	free(content);
	(*i)++;
	return (0);
}
