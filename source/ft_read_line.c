/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/03/24 19:20:05 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_read_line2(char *input, char **envp)
{
	int 	status;
	pid_t	pid;
	char	**cmd;
	t_token	*tokens;

	// Tokenisation de l'entrée utilisateur
	tokens = tokenize(input);
	// TODO: remplacer ft_split() par tokenize() dans l'exécution future
	
	cmd = ft_split(input, ' ');
	pid = fork(); //! je suis pas encore sûr
	if(pid == 0)
	{	
		//! A CHANGER PAR ALGO QUI TEST TOUT LES PATHS //   pour l instant fonctionne uniquement avec les fonction du path: "/bin/"
		if((ft_is_bultin(cmd[0]) == 0))
		{
			execve(ft_path_finder(input) , cmd ,envp);
			perror("execve");
			exit(1);
		}
	}
	else if(pid > 0)
	{
		waitpid(pid, &status, 0); //! pas sûr
	}
	else
		perror("fork"); 
	free(input);
}



void	ft_read_line(char **envp)
{
	char	*input;
	read_history(".minishell_history");
	
	while (1)
	{
		ft_getcwd();
		input = readline("minishell> ");
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
		{
			add_history(input);
		}
		ft_read_line2(input, envp);
	}

	// Sauvegarder l'historique à la fin de la session
	write_history(".minishell_history");

}
