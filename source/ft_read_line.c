/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/06/02 14:13:26 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Effectue la tokenisation et vérifie la syntaxe
static int	handle_token_syntax(t_shell *shell, char *input)
{
	shell->tokens = tokenize(input);
	if (!shell->tokens)
	{
		shell->exit_status = 2;
		return (0);
	}
	if (!is_valid_syntax(shell->tokens))
	{
		free_tokens(shell->tokens);
		shell->exit_status = 2;
		return (0);
	}
	scan_envar_parsing_phase(shell);
	expand_wildcards(shell->tokens);
	return (1);
}

// Traite l'entrée dans la boucle principale
static void	process_input(t_shell *shell, char *input)
{
	if (!*input)
	{
		free(input);
		return ;
	}
	add_history(input);
	save_history(input);
	input = handle_unclosed_quotes(input);
	if (!input)
		return ;
	if (handle_token_syntax(shell, input))
		handle_ast_execution(shell, input);
	else
		free(input);
}

// Lit et traite les entrées utilisateur dans une boucle
static void	ft_read_line_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		init_loop_vars(shell);
		input = readline("minishell> ");
		if (!input)
			break ;
		process_input(shell, input);
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
