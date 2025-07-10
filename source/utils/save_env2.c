/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:06:27 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/10 19:07:27 by lsadikaj         ###   ########.fr       */
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

	fd = open("/home/lsadikaj/Documents/minishell/etc/.minishell_history",
			O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
		perror("open");
	return (fd);
}

void	save_history(char *cmd, t_shell *shell)
{
	int			fd;

	if (should_skip_duplicate(shell->old_cmd, cmd))
		return ;
	fd = open_history_file();
	if (fd == -1)
		return ;
	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	close(fd);
	if (shell->old_cmd)
		free(shell->old_cmd);
	shell->old_cmd = ft_strdup(cmd);
}

void	load_history(void)
{
	int		fd;
	char	*line;

	line = NULL;
	fd = open("/home/lsadikaj/Documents/minishell/etc/.minishell_history",
			O_RDONLY);
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
