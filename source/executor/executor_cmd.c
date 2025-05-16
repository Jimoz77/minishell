/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/14 20:49:55 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Execute la commande dans le processus enfant
static void	exec_child_process(t_node *node, char ***envp)
{
	char	*path;

	// Restaurer les gestionnaires de signaux par défaut
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	path = ft_path_finder(node->cmd[0], envp);
	if (!path)
	{
		//ft_printf("minishell: %s: command not found\n", node->cmd[0]);
		exit(127);
	}
	execve(path, node->cmd, *envp);
	perror("minishell");
	free(path);
	exit(1);
}

// Traite le status de sortie du processus enfant
static int	handle_child_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_and_node(t_node *node, char ***envp)
{
	int	left_status;

	left_status = execute_node_by_type(node->left, envp);
	if (left_status == 0)
	{
		if (node->right)
			return (execute_node_by_type(node->right, envp));
	}
	return (left_status);
}

int	execute_or_node(t_node *node, char ***envp)
{
	int	left_status;

	left_status = execute_node_by_type(node->left, envp);
	if (left_status != 0)
	{
		if (node->right)
			return (execute_node_by_type(node->right, envp));
	}
	return (left_status);
}

// Execute une commande simple (sans pipe ni redirection)
int	execute_cmd_node(t_node *node, char ***envp)
{
	int		status;
	pid_t	pid;

	status = 0;
	if (!node || !node->cmd || !node->cmd[0])
		return (0);
	if (ft_is_builtin(node->cmd, envp))
	{
		return (execute_builtin(node->cmd, envp));
	}
	pid = fork();
	if (pid == 0)
		exec_child_process(node, envp);
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		status = handle_child_status(status);
	}
	else
	{
		perror("minishell: fork");
		status = 1;
	}
	return (status);
}
