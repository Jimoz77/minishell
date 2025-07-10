/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:07:20 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/10 14:48:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	find_systemd_exec_pid_index(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "SYSTEMD_EXEC_PID", 15) == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	**update_pid(char **env)
{
	int		index;
	char	*pid_str;
	char	*new_env;

	pid_str = getenv("SYSTEMD_EXEC_PID");
	if (!pid_str)
		return (env);
	index = find_systemd_exec_pid_index(env);
	if (index != -1)
	{
		new_env = ft_strjoin("SYSTEMD_EXEC_PID=", pid_str);
		free(env[index]);
		env[index] = new_env;
	}
	return (env);
}

static int	open_env_file_write(void)
{
	int	fd;

	fd = open("/home/lsadikaj/Documents/minishell/etc/.mini_env",
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		perror("open");
	return (fd);
}

static void	write_env_to_file(int fd, char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		write(fd, env[i], ft_strlen(env[i]));
		write(fd, "\n", 1);
		i++;
	}
}

void	save_env(char **env)
{
	int	fd;

	fd = open_env_file_write();
	if (fd == -1)
		return ;
	write_env_to_file(fd, env);
	close(fd);
}
