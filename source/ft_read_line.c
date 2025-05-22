/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/05/21 18:22:36 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Effectue la tokenisation et vérifie la syntaxe
static int	handle_token_syntax(t_shell *shell, char *input)
{
	shell->tokens = tokenize(input);
	if (!shell->tokens)
	{
		free(input);
		return (0);
	}
	if (!is_valid_syntax(shell->tokens))
	{
		free_tokens(shell->tokens);
		free(input);
		return (0);
	}
	scan_envar(shell);
	expand_wildcards(shell->tokens);
	return (1);
}

// Prépare les redirections et traite les heredocs
static void	handle_redirections(t_shell *shell)
{
	collect_redirections(shell->tokens, shell);
	process_heredocs(shell);
}

// Gère l'AST et l'exécution de la commande
static void	handle_ast_execution(t_shell *shell, char *input)
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

// Lit et traite les entrées utilisateur dans une boucle
static void	ft_read_line_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->tokens = NULL;
		shell->ast = NULL;
		shell->redirections = NULL;
		shell->heredocs = NULL;
		if (g_signal == 130)
		{
			g_signal = 0;
			rl_on_new_line();
		}
		ft_getcwd();
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			if (handle_token_syntax(shell, input))
			{
				handle_redirections(shell);
				handle_ast_execution(shell, input);
			}
		}
		else
			free(input);
	}
	ft_printf("\n");
}

// Initialise le shell et lance la boucle de lecture
void	ft_read_line(char **envp)
{
	t_shell	*shell;

	shell = init_shell(envp);
	if (!shell)
		return ;
	load_history();
	ft_read_line_loop(shell);
	free_shell(shell);
}
