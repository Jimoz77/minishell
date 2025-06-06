/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 14:22:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/06 17:36:13 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int execute_redir_only(t_node *node)
{
	t_redirect red;

	if(!node || !node->redirections)
		return (0);
	init_redirect(&red);
	process_all_heredocs(node);
	if(!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (0);
}

static int	exec_external(char **cmd, char **envp)
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
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	// Exécuter directement avec execve (on suppose qu'on est déjà dans un fork)
	execve(path, cmd, envp);
	perror("minishell: execve");
	free(path);
	return (126);
}

static int	exec_cmd_with_redirections(t_node *node, char **envp)
{
	t_redirect	red;
	pid_t		pid;
	int			status;

	// TOUJOURS fork pour les commandes externes
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		return (1);
	}
	else if (pid == 0)
	{
		// Processus enfant : appliquer les redirections si nécessaire
		init_redirect(&red);
		if (node->redirections && !apply_node_redirections(node, &red))
		{
			close_redirect_fds(&red);
			restore_std_fds(&red);
			exit(1);
		}
		// Exécuter la commande
		exit(exec_external(node->cmd, envp));
	}
	
	// Processus parent : attendre l'enfant
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	exec_builtin_with_redirections(t_node *node, char ***envp)
{
	t_redirect	red;
	int			result;

	if (!node->redirections)
		return (execute_builtin(node->cmd, envp));
	
	init_redirect(&red);
	if (!apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		return (1);
	}
	result = execute_builtin(node->cmd, envp);
	close_redirect_fds(&red);
	restore_std_fds(&red);
	return (result);
}

int	execute_cmd_node(t_node *node, char ***envp, t_shell *shell)
{
	t_token	*original_tokens;
	t_token	*temp_tokens;
	int		result;

	if (!node)
		return (0);
	
	if((!node->cmd || !node->cmd[0]) && node->redirections)
		return (execute_redir_only(node));
	if((!node->cmd || !node->cmd[0]))
		return (0);
	// Sauvegarder les tokens originaux
	original_tokens = shell->tokens;
	
	// Créer des tokens temporaires pour l'expansion
	temp_tokens = create_tokens_from_cmd(node->cmd, shell);
	/* if (!temp_tokens)
		return (1); */
	
	// Expansion des variables sur les tokens temporaires
	t_token *current = temp_tokens;
	while (current)
	{
		if (!current->parts || (current->parts 
			&& current->parts->type != QUOTE_SINGLE))
		{
			// Utiliser les tokens temporaires pour l'expansion
			shell->tokens = temp_tokens;
			scan_envar_execution_phase(shell, current);
		}
		current = current->next;
	}
	
	// Restaurer immédiatement les tokens originaux
	shell->tokens = original_tokens;
	
	// Mettre à jour la commande avec les valeurs expandées
	update_cmd_from_tokens(node->cmd, temp_tokens);
	
	// Libérer SEULEMENT les tokens temporaires
	free_tokens(temp_tokens);
	
	// Exécuter la commande
	if (ft_is_builtin(node->cmd, envp))
		result = exec_builtin_with_redirections(node, envp);
	else
		result = exec_cmd_with_redirections(node, *envp);
	
	return (result);
}