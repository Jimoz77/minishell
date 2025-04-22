/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/22 16:34:01 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_paren_node(t_node *node, char ***envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0) // Processus enfant
	{
		// Restaurer les gestionnaires de signaux par défaut
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		// Exécuter le contenu des parenthèses
		exit(execute_ast(node->left, envp));
	}
	else if (pid > 0) // Processus parent
	{
		// Attendre la fin du processus enfant
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else // Erreur de fork
	{
		perror("minishell: fork");
	}
	return (1);
}