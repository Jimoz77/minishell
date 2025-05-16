/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/05/16 16:57:16 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Traite l'entrée utilisateur : tokenize, parse, exécute puis libère
static void	process_input(t_shell *shell, char *input)
{
	shell->tokens = tokenize(input);
	if (!shell->tokens)
	{
		free(input);
		return ;
	}
	if (!is_valid_syntax(shell->tokens))
	{
		free_tokens(shell->tokens);
		free(input);
		return ;
	}
	scan_envar(shell);
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast)
	{
		free_tokens(shell->tokens);
		free(input);
		return ;
	}
	shell->exit_status = execute_ast(shell->ast, shell->envp);
	//free_shell(shell);
	free(input);
}

// Boucle principale de lecture de ligne
static void	ft_read_line_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->tokens = NULL;
		shell->ast = NULL;
		if (g_signal == 130)
		{
			g_signal = 0;
			rl_on_new_line();
		}
		ft_getcwd();
		input = readline("minishell> ");
		if (!input)
		{
			ft_printf("\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			save_history(input);
			process_input(shell, input);
			//print_tokens(shell);
		}
		else
			free(input);
	}
}

// Fonction d'initialisation et lancement de la boucle
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
