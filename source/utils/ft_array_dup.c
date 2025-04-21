/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_dup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:52:11 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/21 16:01:33 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_array_dup(char **array)
{
	int		count;
	char	**new;
	int		i;

	count = 0;
	i = 0;
	while (array && array[count])
		count++;
	new = malloc((count + 1) * sizeof(char *));
	if (!new)
		return (NULL);

	while (i < count)
	{
		new[i] = ft_strdup(array[i]);
		i++;
	}
	new[count] = NULL;
	return (new);
}
