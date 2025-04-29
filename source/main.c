/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:59:25 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/28 17:32:37 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char **my_envp;

	my_envp = load_env();
	setup_signals();
	ft_read_line(my_envp);

	return (0);
} */


#include "minishell.h"

static int	execute_command_line(t_shell *shell, char *command)
{
	shell->tokens = tokenize(command);
	if (!is_valid_syntax(shell->tokens))
		return (1);
	shell->ast = parse_ast(shell->tokens);
	return (execute_ast(shell->ast, shell->envp));
}

int	ft_launch_minishell(char *command, char **envp)
{
	t_shell	*shell;
	int		exit_status;

	shell = init_shell(envp); // Modifiez init_shell pour prendre envp
	if (!shell)
		return (127);
	exit_status = execute_command_line(shell, command);
	//free_shell(shell);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	char	**my_envp;

	// Mode non-interactif avec -c
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		int	exit_status = ft_launch_minishell(argv[2], envp);
		exit(exit_status);
	}

	// Mode interactif normal
	my_envp = load_env();
	setup_signals();
	ft_read_line(my_envp);
	return (0);
}