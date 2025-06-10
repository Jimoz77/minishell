/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:13:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/10 18:06:22 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	shell->current_dir = getcwd(NULL, 0);
	if (!shell->current_dir)
	{
		free(shell->envp);
		free(shell);
		return (NULL);
	}
	return (shell);
}

// Libère proprement la structure principale du shell
void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free_array(shell->envp[i++]);
		free(shell->envp);
	}
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->current_dir)
		free(shell->current_dir);
	free(shell);
}
