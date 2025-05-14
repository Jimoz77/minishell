/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:07:20 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/14 20:09:33 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Fonction pour mettre à jour le PID dans l'environnement (peut-etre pas nécessaire)
// fonction qui leak surement
char **update_pid(char **env)
{
	int		i;
	char	*pid_str;
	char	*new_env;

	i = 0;
	pid_str = getenv("SYSTEMD_EXEC_PID");
	if (!pid_str)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "SYSTEMD_EXEC_PID", 15) == 0)
		{
			new_env = ft_strjoin("SYSTEMD_EXEC_PID=", pid_str);
			free(env[i]);
			env[i] = new_env;
			break ;
		}
		i++;
	}
	return (env);
}


void	save_env(char ***env)
{
	int		i;
	int		fd;
//le path ne fonctionne que sur mon ordi // a voir comment le gerer 
// peut etre créé directement dans un repetoir de l'ordi avec la fonction open
	fd = open("/home/jimpa/work/new_mini/etc/.mini_env", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	i = 0;
	if (fd == -1)
	{
		perror("open");
		return ;
	}
	while ((*env)[i])
	{
		write(fd, (*env)[i], ft_strlen((*env)[i]));
		write(fd, "\n", 1);
		i++;
	}
	close(fd);
}

char	**load_env(void)
{
	int			fd;
	ssize_t		bytes;
	int			count;
	char		buffer[4096];
	char		**env;
	char		*line;

	fd = open("/home/jimpa/work/new_mini/etc/.mini_env", O_RDONLY); // Même chemin
	if (fd == -1){
		perror("open");
		return (NULL);
	}
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes <= 0)
		return (NULL);
	buffer[bytes] = '\0';

	// Découpage des lignes
	count = 0;
	env = malloc(sizeof(char *));
	line = ft_strtok(buffer, "\n");
	while (line)
	{
		env[count] = ft_strdup(line);
		env = realloc(env, sizeof(char *) * (++count + 1));  // a implementer
		line = ft_strtok(NULL, "\n");
	}
	env[count] = NULL;
	return (update_pid(env));
}

void	save_history(char *cmd)
{
	int		fd;
	static char *old_cmd = {NULL};
	if (old_cmd && ft_strncmp(old_cmd, cmd, ft_strlen(cmd)) == 0 && ft_strncmp(old_cmd, cmd, ft_strlen(old_cmd)) == 0)
		return ;
//le path ne fonctionne que sur mon ordi // a voir comment le gerer 
// peut etre créé directement dans un repetoir de l'ordi avec la fonction open
	fd = open("/home/jimpa/work/new_mini/etc/.minishell_history", O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("open");
		return ;
	}

	write(fd, cmd, ft_strlen(cmd));
	write(fd, "\n", 1);
	close(fd);
	free(old_cmd);
	old_cmd = ft_strdup(cmd);
}

void    load_history(void)
{
    int     fd;
    char    *line = NULL;

	fd = open("/home/jimpa/work/new_mini/etc/.minishell_history", O_RDONLY);
    if (fd == -1)
		return;
	 while ((line = get_next_line(fd)) != NULL)
    {
        if (*line)
            add_history(line);
        free(line);
        line = NULL;
    }
    
    close(fd);
}