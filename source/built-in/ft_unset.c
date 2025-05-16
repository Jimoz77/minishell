/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:46:40 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/14 20:49:08 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// pas encore fonctionnel
int	ft_unset(char **cmd, char ***envp)
{
	int	exit_status;
	int	i;
	int	j;
	int	k;
	int	len;
	int	found;

	exit_status = 0;
	i = 1;
	while (cmd[i])
	{
		if (!is_valid_id(cmd[i]))
		{
			write(STDERR_FILENO, "unset: '", 8);
			write(STDERR_FILENO, cmd[i], ft_strlen(cmd[i]));
			write(STDERR_FILENO, "': not a valid indentifier\n", 25);
			exit_status = 1;
		}
		else
		{
			j = 0;
			found = 0;
			len = ft_strlen(cmd[i]);
			while ((*envp)[j] && !found)
			{
				if (ft_strncmp((*envp)[j], cmd[i], len) == 0 && (*envp)[j][len] == '=')
				{
					free((*envp)[j]);
					k = j;
					while ((*envp)[k + 1] != NULL)
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
		i++;
	}
	return (exit_status);
}
