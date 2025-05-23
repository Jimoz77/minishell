/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:15:23 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/21 14:15:46 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Initialise les variables de la boucle
void	init_loop_vars(t_shell *shell)
{
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
void	handle_ast_execution(t_shell *shell, char *input)
{
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast)
	{
		free_tokens(shell->tokens);
		free_redirections(shell->redirections);
		free_heredocs(shell->heredocs);
		shell->redirections = NULL;
		shell->heredocs = NULL;
		free(input);
		return ;
	}
	shell->exit_status = execute_ast(shell->ast, shell->envp, shell);
	free_tokens(shell->tokens);
	free_redirections(shell->redirections);
	free_heredocs(shell->heredocs);
	shell->redirections = NULL;
	shell->heredocs = NULL;
	free(input);
}
