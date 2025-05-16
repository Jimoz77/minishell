/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:59:25 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/23 18:21:55 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* int	main(int argc, char **argv, char **envp)
{
	char	**my_envp;

	(void)argc;
	(void)argv;
	(void)envp;
	my_envp = load_env();
	if (!my_envp)
		my_envp = ft_array_dup(envp);
	setup_signals();
	ft_read_line(my_envp);
	return (0);
} */

static int	execute_command_line(t_shell *shell, char *command)
{
  int exit_status:
  
	shell->tokens = tokenize(command);
	if (!is_valid_syntax(shell->tokens))
	{
		free_tokens(shell->tokens);
		return (1);
	}
	scan_envar(shell); // Expansion des variables
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast) // Gestion d'erreur du parsing AST
	{
		free_tokens(shell->tokens);
		return (1);
	}
	exit_status = execute_ast(shell->ast, shell->envp);
	
	// Nettoyage
	free_tokens(shell->tokens);
	free_ast(shell->ast);
	return exit_status;
}

int	ft_launch_minishell(char *command, char **envp)
{
	t_shell   *shell;
	int       exit_status;

	shell = init_shell(envp);
	if (!shell)
		return (127);
	exit_status = execute_command_line(shell, command);
	//free_shell(shell); // Décommenté
	return (exit_status);
}

int	main(int argc, char **argv)
{
	// Mode non-interactif avec -c
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		char	**my_envp = load_env(); // Chargement cohérent de l'environnement
		setup_signals(); // Configuration des signaux
		int	exit_status = ft_launch_minishell(argv[2], my_envp);
		exit(exit_status);
	}
	// Mode interactif normal
	char	**my_envp = load_env();
	setup_signals();
	ft_read_line(my_envp);
	return (0);
}
