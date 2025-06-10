/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:05:02 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 19:26:26 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_id(char *str)
{
	int	i;

	i = 1;
	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	**ft_array_add(char **array, const char *str)
{
	int		count;
	char	**new;
	int		i;

	count = 0;
	i = 0;
	while (array[count])
		count++;
	new = malloc((count + 2) * sizeof(char *));
	if (!new)
		return (NULL);
	while (i < count)
	{
		new[i] = array[i];
		i++;
	}
	new[count] = ft_strdup(str);
	new[count + 1] = NULL;
	free(array);
	return (new);
}

int	get_envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}
