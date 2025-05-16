/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_combined.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:28:14 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 15:09:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Forke et exécute une commande avec redirections
static int	fork_and_execute_cmd(t_node *cmd_node, char **envp,
	t_redirect *red)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		close_redirect_fds(red);
		restore_std_fds(red);
		return (1);
	}
	if (pid == 0)
		exit(execute_cmd_node(cmd_node, &envp));
	waitpid(pid, &status, 0);
	close_redirect_fds(red);
	restore_std_fds(red);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

// Exécute un nœud combiné (commande + redirections)
int	execute_combined_node(t_node *node, char **envp, t_shell *shell)
{
	t_node		*cmd_node;
	t_redirect	red;

	cmd_node = find_command_node(node);
	if (!cmd_node)
		return (1);
	init_redirect(&red);
	process_heredocs(shell);
	if (!apply_all_redirections(shell, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	return (fork_and_execute_cmd(cmd_node, envp, &red));
}
