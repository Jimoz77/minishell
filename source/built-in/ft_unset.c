/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:46:40 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 19:24:56 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Vérifie si une chaîne est
//un identifiant valide pour une variable d’environnement
static int	is_valid_idd(char *str)
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static void	remove_env_var(char *var, char ***envp)
{
	int	j;
	int	k;
	int	len;
	int	found;

	j = 0;
	found = 0;
	len = ft_strlen(var);
	while ((*envp)[j] && !found)
	{
		if (ft_strncmp((*envp)[j], var, len) == 0 && (*envp)[j][len] == '=')
		{
			free((*envp)[j]);
			k = j;
			while ((*envp)[k + 1])
			{
				(*envp)[k] = (*envp)[k + 1];
				k++;
			}
			(*envp)[k] = NULL;
			found = 1;
		}
		else
			j++;
	}
}

int	ft_unset(char **cmd, char ***envp)
{
	int	i;

	i = 1;
	while (cmd[i])
	{
		if (!is_valid_idd(cmd[i]))
		{
			write(STDERR_FILENO, "unset: '", 8);
			write(STDERR_FILENO, cmd[i], ft_strlen(cmd[i]));
			write(STDERR_FILENO, "': not a valid indentifier\n", 27);
		}
		else
			remove_env_var(cmd[i], envp);
		i++;
	}
	return (0);
}
