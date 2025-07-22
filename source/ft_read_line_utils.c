/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:15:23 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/21 14:57:26 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_loop_vars(t_shell *shell)
{
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->last_used_token = NULL;
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
	(void)input;
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast)
	{
		free_tokens(shell->tokens);
		return ;
	}
	shell->exit_status = execute_ast(shell->ast, shell->envp, shell);
	free_tokens(shell->tokens);
	free_ast(shell->ast);
}
