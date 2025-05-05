/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 16:45:15 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Exécute un sous-arbre entre parenthèses dans un sous-processus
int	execute_paren_node(t_node *node, char ***envp, t_shell *shell)
{
	int		status;
	pid_t	child_pid;

	if (!node || !node->left)
		return (1);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (child_pid == 0)
	{
		// Enfant : exécute la sous-commande entre parenthèses
		exit(execute_ast(node->left, envp, shell));
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
