/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 23:42:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/20 22:29:18 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Libère un tableau de chaînes terminé par NULL
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

// Libère un tableau de chaînes (alias de free_array)
void	ft_free_split(char **split_array)
{
	free_array(split_array);
}

char	*ft_strjoin_free(char *s1, const char *s2)
{
	char	*result;

	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

void	free_t_word_parts(t_token *token)
{
	t_word_part	*head;
	t_word_part	*current;

	if (!token || !token->parts)
		return ;
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

	if (!tokens)
		return ;
	current = tokens;
	while (current)
	{
		next = current->next;
		if (current->value)
		{
			free(current->value);
			current->value = NULL;
		}
		if (current->parts)
		{
			free_t_word_parts(current);
			current->parts = NULL;
		}
		free(current);
		current = next;
	}
}
