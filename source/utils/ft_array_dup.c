/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:52:11 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/27 18:52:26 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


char	***ft_array_dup2(char ***array)
{
	int		count;
	char	***new;
	int		i;

	count = 0;
	i = 0;
	if (!array)
		return (NULL);
	while (array && array[count])
		count++;
	new = malloc((count + 1) * sizeof(char **));
	if (!new)
		return (NULL);
	while (i < count)
	{
		new[i] = ft_array_dup(array[i]);
		if (!new[i])
		{
			while (--i > 0)
				free_array(new[i]);
			free(new);
			return (NULL);
		}
		i++;
	}
	new[count] = NULL;
	return (new);
}



char	**ft_array_dup(char **array)
{
	int		count;
	char	**new;
	int		i;

	count = 0;
	i = 0;
	if (!array)
		return (NULL);
	while (array && array[count])
		count++;
	new = malloc((count + 1) * sizeof(char *));
	if (!new)
		return (NULL);
	while (i < count)
	{
		new[i] = ft_strdup(array[i]);
		if (!new[i])
		{
			while (--i > 0)
				free(new[i]);
			free(new);
			return (NULL);
		}
		i++;
	}
	new[count] = NULL;
	return (new);
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