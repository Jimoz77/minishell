/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:07:22 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/17 21:23:20 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	read_env_file(char *buffer)
{
	int	fd;
	int	bytes;

	fd = open("/home/leotrimsadikaj/Documents/Cursus42/minishell/etc/.mini_env", O_RDONLY);
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

	env = ft_calloc(1, sizeof(char *));
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

char	**process_env_lines(char *buffer)
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
