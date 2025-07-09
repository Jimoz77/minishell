/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/09 14:58:13 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	child_paren_exec(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	setup_exec_signals();
	init_redirect(&red);
	if (node->redirections && !apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		exit(1);
	}
	exit(execute_ast(node->left, envp, shell));
}

static int	fork_and_execute(t_node *node, char ***envp, t_shell *shell)
{
	pid_t	child_pid;
	int		status;
	int		sig;

	setup_exec_signals();
	child_pid = fork();
	if (child_pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	if (child_pid == 0)
		child_paren_exec(node, envp, shell);
	waitpid(child_pid, &status, 0);
	restore_signals();
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (128 + sig);
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

static int	execute_with_isol_env(t_node *node, char ***envp, t_shell *shell)
{
	char	***sub_envp;
	char	***original_envp;
	int		result;

	sub_envp = ft_array_dup2(envp);
	if (!sub_envp)
		return (1);
	original_envp = shell->envp;
	shell->envp = sub_envp;
	result = execute_ast(node->left, sub_envp, shell);
	shell->envp = original_envp;
	if (sub_envp && *sub_envp)
	{
		free_array(*sub_envp);
		free(sub_envp);
	}
	return (result);
}

int	execute_paren_node(t_node *node, char ***envp, t_shell *shell)
{
	if (!node || !node->left)
		return (1);
	if (!node->redirections)
		return (execute_with_isol_env(node, envp, shell));
	return (fork_and_execute(node, envp, shell));
}
