/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:07:20 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/21 18:18:29 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	save_env(char ***env)
{
	int		i;
	int		fd;
//le path ne fonctionne que sur mon ordi // a voir comment le gerer 
// peut etre créé directement dans un repetoir de l'ordi avec la fonction open
	fd = open("/home/jimpa/workspace/gitdump/minishell/etc/.mini_env", O_WRONLY | O_CREAT | O_TRUNC, 0644);
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

	fd = open("/home/jimpa/workspace/gitdump/minishell/etc/.mini_env", O_RDONLY); // Même chemin
	if (fd == -1)
		return (NULL);
	bytes = read(fd, buffer, sizeof(buffer) - 1);
	close(fd);
	if (bytes <= 0)
		return (NULL);
	buffer[bytes] = '\0';

	// Découpage des lignes
	count = 0;
	env = malloc(sizeof(char *));
	line = strtok(buffer, "\n");
	while (line)
	{
		env[count] = strdup(line);
		env = realloc(env, sizeof(char *) * (++count + 1));
		line = strtok(NULL, "\n");
	}
	env[count] = NULL;
	return (env);
}
