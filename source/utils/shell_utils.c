/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:13:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 10:59:49 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initialise la structure principale du shell
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
	shell->heredocs = NULL;
	shell->redirections = NULL;
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
			free(shell->envp[i++]);
		free(shell->envp);
	}
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->heredocs)
		free_heredocs(shell->heredocs);
	if (shell->redirections)
		free_redirections(shell->redirections);
	free(shell);
}
