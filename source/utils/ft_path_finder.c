/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_finder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:13:51 by jiparcer          #+#    #+#             */
/*   Updated: 2025/04/21 15:49:45 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_path_finder(char *cmd, char ***envp)
{
	char	*path;
	char	**path_list;
	int		i;
	char	**tmp;
	char	*dir;

	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "PATH=", 5) != 0)
		i++;

	if (!(*envp)[i])
	{
		printf("PATH not found\n");
		return (NULL);
	}
	tmp = ft_split((*envp)[i], '=');
	if (!tmp[1])
	{
		ft_free_split(tmp);
		printf("PATH is empty\n");
		return (NULL);
	}
	path_list = ft_split(tmp[1], ':');
	ft_free_split(tmp);
	i = 0;
	while (path_list[i])
	{
		dir = path_list[i];
		if (dir[0] == '\0')
			dir = ".";
		path = ft_strjoin(dir, "/");
		path = ft_strjoin_free(path, cmd);
		if (access(path, X_OK) == 0)
		{
			ft_free_split(path_list);
			return (path);
		}
		free(path);
		i++;
	}
	ft_free_split(path_list);
	return (NULL);
}
