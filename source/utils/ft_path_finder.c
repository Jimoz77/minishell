/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_path_finder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 15:13:51 by jiparcer          #+#    #+#             */
/*   Updated: 2025/07/03 15:04:31 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	find_path_env_index(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	return (i);
}

static char	**extract_path_list(char **envp, int path_index)
{
	char	**tmp;
	char	**path_list;

	if (!envp[path_index])
	{
		printf("PATH not found\n");
		return (NULL);
	}
	tmp = ft_split(envp[path_index], '=');
	if (!tmp[1])
	{
		ft_free_split(tmp);
		printf("PATH is empty\n");
		return (NULL);
	}
	path_list = ft_split(tmp[1], ':');
	ft_free_split(tmp);
	return (path_list);
}

static char	*build_full_path(char *dir, char *cmd)
{
	char	*path;
	char	*current_dir;

	current_dir = dir;
	if (dir[0] == '\0')
		current_dir = ".";
	path = ft_strjoin(current_dir, "/");
	path = ft_strjoin_free(path, cmd);
	return (path);
}

static char	*search_in_path_list(char **path_list, char *cmd)
{
	char	*path;
	int		i;

	i = 0;
	while (path_list[i])
	{
		path = build_full_path(path_list[i], cmd);
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

char	*ft_path_finder(char *cmd, char ***envp)
{
	char	**path_list;
	char	*trimmed_cmd;
	char	*result;
	int		path_index;

	path_index = find_path_env_index(*envp);
	path_list = extract_path_list(*envp, path_index);
	if (!path_list)
		return (NULL);
	trimmed_cmd = trim_left(cmd);
	result = search_in_path_list(path_list, trimmed_cmd);
	return (result);
}
