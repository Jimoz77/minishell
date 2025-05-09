/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 15:06:45 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initialise les redirections et retourne 0 si erreur
static int	setup_parent_redir(t_redirect *red, t_shell *shell)
{
	init_redirect(red);
	if (shell->redirections && !apply_all_redirections(shell, red))
	{
		close_redirect_fds(red);
		restore_std_fds(red);
		return (0);
	}
	return (1);
}

// Gère le processus enfant pour exécuter node->left
static int	exec_child_process(t_node *node, char ***envp, t_shell *shell)
{
	exit(execute_ast(node->left, envp, shell));
}

// Exécute un sous-arbre entre parenthèses dans un sous-processus
int	execute_paren_node(t_node *node, char ***envp, t_shell *shell)
{
	int			status;
	pid_t		child_pid;
	t_redirect	red;

	if (!node || !node->left)
		return (1);
	if (!setup_parent_redir(&red, shell))
		return (1);
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("minishell: fork");
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	if (child_pid == 0)
		exec_child_process(node, envp, shell);
	waitpid(child_pid, &status, 0);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
