/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/17 21:04:37 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	execute_redir_only(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	(void)envp;
	(void)shell;
	init_redirect(&red);
	if (!node || !node->redirections)
		return (0);
	process_all_heredocs_expanded(node, shell);
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	if (node->left)
		node->left->redirections = node->redirections;
	if (node->right)
		node->right->redirections = node->redirections;
	close_redirect_fds(&red);
	restore_std_fds(&red);
	if (node->left && (node->left->type == NODE_AND
			|| node->left->type == NODE_OR))
		execute_ast(node->left, envp, shell);
	return (0);
}

static int	exec_external(char **cmd, char **envp, t_shell *shell)
{
	char	*path;
	int		special;

	if (!cmd || !cmd[0])
		return (1);
	special = handle_special_commands(cmd);
	if (special)
		return (special);
	path = handle_direct_path(cmd[0]);
	if (!path)
		path = ft_path_finder(cmd[0], &envp);
	if (!path)
	{
		if (shell->tokens->type == TOKEN_REDIRECT_OUT)
			return (127);
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_shell(shell);
		free(path);
		return (127);
	}
	execve(path, cmd, envp);
	perror("minishell: execve");
	free(path);
	return (126);
}

void	child_exec_process(t_node *node, char **envp, t_shell *shell)
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
	exit(exec_external(node->cmd, envp, shell));
}

int	handle_exec_status(int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(1, "Quit (core dumped)\n", 19);
		return (128 + sig);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_cmd_node(t_node *node, char ***envp, t_shell *shell)
{
	if (!node)
		return (0);
	if ((!node->cmd || !node->cmd[0]) && node->redirections)
		return (execute_redir_only(node, envp, shell));
	if (!node->cmd || !node->cmd[0])
		return (0);
	return (process_cmd_tokens(node, envp, shell));
}
