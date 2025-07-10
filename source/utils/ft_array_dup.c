/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:52:11 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/10 18:09:29 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	***ft_array_dup2(char ***array)
{
	t_ft_array_dup2	v;

	v.count = 0;
	v.i = 0;
	if (!array)
		return (NULL);
	while (array && array[v.count])
		v.count++;
	v.new = malloc((v.count + 1) * sizeof(char **));
	if (!v.new)
		return (NULL);
	while (v.i < v.count)
	{
		v.new[v.i] = ft_array_dup(array[v.i]);
		if (!v.new[v.i])
		{
			while (--v.i > 0)
				free_array(v.new[v.i]);
			free(v.new);
			return (NULL);
		}
		v.i++;
	}
	v.new[v.count] = NULL;
	return (v.new);
}

char	**ft_array_dup(char **array)
{
	t_ft_array_dup	v;

	v.count = 0;
	v.i = 0;
	if (!array)
		return (NULL);
	while (array && array[v.count])
		v.count++;
	v.new = malloc((v.count + 1) * sizeof(char *));
	if (!v.new)
		return (NULL);
	while (v.i < v.count)
	{
		v.new[v.i] = ft_strdup(array[v.i]);
		if (!v.new[v.i])
		{
			while (--v.i > 0)
				free(v.new[v.i]);
			free(v.new);
			return (NULL);
		}
		v.i++;
	}
	v.new[v.count] = NULL;
	return (v.new);
}

char	***ft_wrap_array(char **array)
{
	char	***new;

	if (!array)
		return (NULL);
	new = malloc(2 * sizeof(char **));
	if (!new)
		return (NULL);
	new[0] = ft_array_dup(array);
	new[1] = NULL;
	return (new);
}
