/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:25:33 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 17:21:01 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

char	*handle_direct_path(char *cmd)
{
	if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		return (ft_strdup(cmd));
	return (NULL);
}

void	update_cmd_from_tokens(char **cmd, t_token *tokens)
{
	t_token	*current;
	int		i;

	current = tokens;
	i = 0;
	while (current && cmd[i])
	{
		free(cmd[i]);
		cmd[i] = ft_strdup(current->value);
		current = current->next;
		i++;
	}
}

int	handle_special_commands(char **cmd)
{
	if (ft_strcmp(cmd[0], ".") == 0)
	{
		ft_putstr_fd("minishell: .: filename argument required\n", 2);
		ft_putstr_fd(".: usage: . filename [arguments]\n", 2);
		return (2);
	}
	if (ft_strcmp(cmd[0], "..") == 0)
	{
		ft_putstr_fd("minishell: ..: command not found\n", 2);
		return (127);
	}
	return (0);
}

int	execute_with_path(char *path, char **cmd, char **envp)
{
	if (is_directory(path))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Is a directory", 2);
		free(path);
		return (126);
	}
	if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": Permission denied", 2);
		free(path);
		return (126);
	}
	execve(path, cmd, envp);
	perror("minishell");
	free(path);
	return (126);
}
