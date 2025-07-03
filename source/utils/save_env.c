/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:07:20 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 01:41:19 by jimpa            ###   ########.fr       */
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

	fd = open("/home/jimpa/work/der_der_mini/etc/.mini_env",
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

void	save_env(char ***env)
{
	int	fd;

	fd = open_env_file_write();
	if (fd == -1)
		return ;
	write_env_to_file(fd, *env);
	close(fd);
}

static int	read_env_file(char *buffer)
{
	int	fd;
	int	bytes;

	fd = open("/home/jimpa/work/der_der_mini/etc/.mini_env", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}
	bytes = read(fd, buffer, 4095);
	close(fd);
	if (bytes <= 0)
		return (-1);
	buffer[bytes] = '\0';
	return (bytes);
}

static char	**allocate_initial_env(void)
{
	char	**env;

	env = malloc(sizeof(char *) * 1);
	if (!env)
		return (NULL);
	return (env);
}

static void	free_env_array(char **env, int count)
{
	while (count > 0)
		free(env[--count]);
	free(env);
}

static char	**resize_env_array(char **env, int count, int new_size)
{
	char	**new_env;

	new_env = ft_realloc(env, (count + 1) * sizeof(char *),
			new_size * sizeof(char *));
	if (!new_env)
		free_env_array(env, count);
	return (new_env);
}

static char	**process_env_lines(char *buffer)
{
	t_process_env_l	v;

	v.count = 0;
	v.current_size = 1;
	v.env = allocate_initial_env();
	if (!v.env)
		return (NULL);
	v.line = ft_strtok(buffer, '\n');
	while (v.line)
	{
		if ((size_t)v.count + 1 >= (size_t)v.current_size)
		{
			v.current_size *= 2;
			v.env = resize_env_array(v.env, v.count, v.current_size);
			if (!v.env)
				return (NULL);
		}
		v.env[v.count] = ft_strdup(v.line);
		if (!v.env[v.count])
			return (free_env_array(v.env, v.count), NULL);
		v.count++;
		v.line = ft_strtok(NULL, '\n');
	}
	v.env[v.count] = NULL;
	return (v.env);
}

char	**load_env(void)
{
	char	buffer[4096];
	char	**env;

	if (read_env_file(buffer) == -1)
		return (NULL);
	env = process_env_lines(buffer);
	if (!env)
		return (NULL);
	return (update_pid(env));
}

static int	should_skip_duplicate(char *old_cmd, char *cmd)
{
	if (!old_cmd)
		return (0);
	if (ft_strncmp(old_cmd, cmd, ft_strlen(cmd)) == 0
		&& ft_strncmp(old_cmd, cmd, ft_strlen(old_cmd)) == 0)
		return (1);
	return (0);
}

static int	open_history_file(void)
{
	int	fd;

	fd = open("/home/jimpa/work/der_der_mini/etc/.minishell_history",
			O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		perror("open");
	return (fd);
}

void	save_history(char *cmd)
{
	int			fd;
	static char	*old_cmd = NULL;

	if (should_skip_duplicate(old_cmd, cmd))
		return ;
	fd = open_history_file();
	if (fd == -1)
		return ;
	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	close(fd);
	free(old_cmd);
	old_cmd = ft_strdup(cmd);
}

void	load_history(void)
{
	int		fd;
	char	*line;

	line = NULL;
	fd = open("/home/jimpa/work/der_der_mini/etc/.minishell_history", O_RDONLY);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (*line)
			add_history(line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}
