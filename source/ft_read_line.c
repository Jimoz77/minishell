/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/07/22 13:04:40 by jimpa            ###   ########.fr       */
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
		return ;
	add_history(input);
	save_history(input, shell);
	//input = handle_unclosed_quotes(input);
	if (ft_strncmp(input, "./minishell", 11) == 0)
		increment_shlvl(shell->envp);
	/* if (!input)
		return ; */
	if (handle_token_syntax(shell, input))
	{
		handle_ast_execution(shell, input);
		if (ft_strncmp(input, "./minishell", 10) == 0)
			decrement_shlvl(shell->envp);
	}
}

// Lit et traite les entrées utilisateur dans une boucle
static void	ft_read_line_loop(t_shell *shell)
{
	char	*input;
	char	*processed_input;

	input = NULL;
	while (1)
	{
		init_loop_vars(shell);
		input = readline("minishell> ");
		if (!input)
			break ;
		processed_input = handle_unclosed_quotes(input);
		if (processed_input)
		{
			process_input(shell, processed_input);
			free(processed_input);
		}
		else if (input)
		{
			free(input);
		}
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
	/* if (envp)
		free_array(envp); */
}
