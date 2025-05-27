/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:15:23 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/26 18:56:14 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Initialise les variables de la boucle
void	init_loop_vars(t_shell *shell)
{
	// S'assurer que tout est NULL avant de commencer
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (shell->redirections)
	{
		free_redirections(shell->redirections);
		shell->redirections = NULL;
	}
	if (shell->heredocs)
	{
		free_heredocs(shell->heredocs);
		shell->heredocs = NULL;
	}
	
	// Réinitialiser à NULL (au cas où)
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->redirections = NULL;
	shell->heredocs = NULL;
	
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		rl_on_new_line();
	}
	ft_getcwd();
}

// Gère l'AST et l'exécution de la commande
void	handle_ast_execution(t_shell *shell)
{
	t_redirect	red;
	
	init_redirect(&red);
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast)
	{
		restore_std_fds(&red);
		return ;
	}
	shell->exit_status = execute_ast(shell->ast, shell->envp, shell);
	restore_std_fds(&red);
}
