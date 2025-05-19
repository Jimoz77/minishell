/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/19 18:31:43 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Exécute une commande externe en cherchant son chemin dans $PATH
static int	exec_external(char **cmd, char **envp)
{
	char	*path;

	if (!cmd || !cmd[0])
		return (1);
	path = ft_path_finder(cmd[0], &envp);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	execve(path, cmd, envp);
	perror("minishell");
	free(path);
	return (126);
}

// Forke et exécute une commande dans un processus enfant
static int	exec_forked(t_node *node, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
		exit(exec_external(node->cmd, envp));
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

// Exécute un nœud de type commande (builtin ou externe)
int	execute_cmd_node(t_node *node, char ***envp)
{
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	if (ft_is_builtin(node->cmd, envp))
		return (execute_builtin(node->cmd, envp));
	return (exec_forked(node, *envp));
}
