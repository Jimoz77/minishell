/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:13:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/10 18:35:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Copie old_size ou new_size octets depuis ptr vers new_ptr
static void	copy_and_free_old(void *new_ptr, void *ptr,
			size_t copy_size)
{
	size_t	i;

	i = 0;
	while (i < copy_size)
	{
		((char *)new_ptr)[i] = ((char *)ptr)[i];
		i++;
	}
	free(ptr);
}

// Réalloue une zone mémoire en copiant les données existantes
// Fonction pour obtenir un PID alternatif sans utiliser getpid()
// Fallback: utiliser une valeur basée sur l'adresse mémoire

int	get_shell_pid2(int shell_pid, int fd)
{
	char		buffer[32];
	ssize_t		bytes_read;
	int			i;

	if (fd != -1)
	{
		bytes_read = read(fd, buffer, sizeof(buffer) - 1);
		close(fd);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			i = 0;
			while (buffer[i] && buffer[i] != ' ')
				i++;
			buffer[i] = '\0';
			shell_pid = ft_atoi(buffer);
			if (shell_pid > 0)
				return (shell_pid);
		}
	}
	shell_pid = (int)((unsigned long)&shell_pid % 32768) + 1000;
	return (shell_pid);
}

int	get_shell_pid(void)
{
	int			fd;
	static int	shell_pid = 0;

	if (shell_pid != 0)
		return (shell_pid);
	fd = open("/proc/self/stat", O_RDONLY);
	return (get_shell_pid2(shell_pid, fd));
}

// Initialise la structure principale du shell
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	copy_size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (ptr)
	{
		copy_size = old_size;
		if (new_size < old_size)
			copy_size = new_size;
		copy_and_free_old(new_ptr, ptr, copy_size);
	}
	return (new_ptr);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;

	shell = ft_calloc(1, sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = ft_wrap_array(envp);
	if (!shell->envp)
	{
		free(shell);
		return (NULL);
	}
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->exit_status = 0;
	shell->last_used_token = NULL;
	shell->current_dir = getcwd(NULL, 0);
	shell->old_cmd = NULL;
	shell->heredoc_interrupted = 0;
	if (!shell->current_dir)
	{
		free(shell->envp);
		free(shell);
		return (NULL);
	}
	return (shell);
}
