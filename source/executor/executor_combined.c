/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_combined.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:28:14 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 18:48:30 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Initialise la structure de redirection avec des valeurs par défaut */
static void	init_redirect(t_redirect *red)
{
	red->stdin_fd = -1;
	red->stdout_fd = -1;
	red->saved_stdin = dup(STDIN_FILENO);
	red->saved_stdout = dup(STDOUT_FILENO);
}

/* Ferme les descripteurs ouverts dans les redirections */
static void	close_redirect_fds(t_redirect *red)
{
	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
	if (red->stdout_fd != -1)
	{
		close(red->stdout_fd);
		red->stdout_fd = -1;
	}
}

/* Restaure les descripteurs standard à leurs valeurs originales */
static void	restore_std_fds(t_redirect *red)
{
	if (red->saved_stdin != -1)
	{
		dup2(red->saved_stdin, STDIN_FILENO);
		close(red->saved_stdin);
		red->saved_stdin = -1;
	}
	if (red->saved_stdout != -1)
	{
		dup2(red->saved_stdout, STDOUT_FILENO);
		close(red->saved_stdout);
		red->saved_stdout = -1;
	}
}

/* Execute un nœud combiné (commande avec redirections) */
int	execute_combined_node(t_node *node, char **envp, t_shell *shell)
{
	t_node		*cmd_node;
	t_redirect	red;
	pid_t		pid;
	int			status;

	cmd_node = find_command_node(node);
	if (!cmd_node)
		return (1);
	init_redirect(&red);
	if (!setup_all_redirects(node, &red, shell))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	if (pid == 0)
	{
		// Dans le fils, les redirections sont déjà appliquées
		exit(execute_cmd_node(cmd_node, &envp));
	}
	waitpid(pid, &status, 0);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
