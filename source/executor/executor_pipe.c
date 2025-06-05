/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/05 17:13:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Fonction spéciale pour exécuter dans un contexte de pipe (sans fork)
static int	execute_in_pipe_context(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;
	t_token		*original_tokens;
	t_token		*temp_tokens;

	if (!node)
		return (1);
	
	// Si c'est un NODE_CMD, on doit gérer l'expansion et les redirections
	if (node->type == NODE_CMD)
	{
		if (!node->cmd || !node->cmd[0])
			return (0);
		
		// Sauvegarder et expansion (même logique que execute_cmd_node)
		original_tokens = shell->tokens;
		temp_tokens = create_tokens_from_cmd(node->cmd, shell);
		if (!temp_tokens)
			return (1);
		
		// Expansion
		t_token *current = temp_tokens;
		while (current)
		{
			if (!current->parts || (current->parts 
				&& current->parts->type != QUOTE_SINGLE))
			{
				shell->tokens = temp_tokens;
				scan_envar_execution_phase(shell, current);
			}
			current = current->next;
		}
		
		shell->tokens = original_tokens;
		update_cmd_from_tokens(node->cmd, temp_tokens);
		free_tokens(temp_tokens);
		
		// Appliquer les redirections
		init_redirect(&red);
		if (node->redirections && !apply_node_redirections(node, &red))
		{
			close_redirect_fds(&red);
			restore_std_fds(&red);
			return (1);
		}
		
		// Si builtin
		if (ft_is_builtin(node->cmd, envp))
		{
			int result = execute_builtin(node->cmd, envp);
			close_redirect_fds(&red);
			restore_std_fds(&red);
			return (result);
		}
		
		// Si commande externe, execve directement (pas de fork)
		char *path = handle_direct_path(node->cmd[0]);
		if (!path)
			path = ft_path_finder(node->cmd[0], envp);
		if (!path)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(node->cmd[0], 2);
			ft_putendl_fd(": command not found", 2);
			return (127);
		}
		
		execve(path, node->cmd, *envp);
		perror("minishell: execve");
		free(path);
		return (126);
	}
	else
	{
		// Pour les autres types de nœuds, appeler la fonction normale
		return (execute_node_by_type(node, envp, shell));
	}
}

static int	exec_pipe_left(t_node *node, char ***envp, int pipefd[2],
	t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		// Utiliser la fonction spéciale pour éviter le double fork
		exit(execute_in_pipe_context(node->left, envp, shell));
	}
	return (pid);
}

static int	exec_pipe_right(t_node *node, char ***envp, int pipefd[2],
	t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		// Utiliser la fonction spéciale pour éviter le double fork
		exit(execute_in_pipe_context(node->right, envp, shell));
	}
	return (pid);
}

static int	wait_for_pipes(pid_t pid_left, pid_t pid_right)
{
	int	status;

	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	execute_pipe_node(t_node *node, char ***envp, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		return (1);
	}
	pid_left = exec_pipe_left(node, envp, pipefd, shell);
	if (pid_left == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	pid_right = exec_pipe_right(node, envp, pipefd, shell);
	close(pipefd[0]);
	close(pipefd[1]);
	if (pid_right == -1)
		return (1);
	return (wait_for_pipes(pid_left, pid_right));
}