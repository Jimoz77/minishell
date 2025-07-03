/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:06:27 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:06:36 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
