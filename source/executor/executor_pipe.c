/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 16:35:18 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/24 18:02:51 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Crée une copie filtrée des redirections (seulement les redirections d'entrée)
static t_redir	*filter_input_redirections(t_redir *all_redirections)
{
	t_redir	*filtered;
	t_redir	*current;

	filtered = NULL;
	current = all_redirections;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_IN\
		|| current->type == TOKEN_HEREDOC)
			add_redirection(&filtered, current->type, current->filename);
		current = current->next;
	}
	return (filtered);
}

// Crée une copie filtrée des redirections (seulement les redirections de sortie)
static t_redir	*filter_output_redirections(t_redir *all_redirections)
{
	t_redir	*filtered;
	t_redir	*current;

	filtered = NULL;
	current = all_redirections;
	while (current)
	{
		if (current->type == TOKEN_REDIRECT_OUT\
|| current->type == TOKEN_APPEND)
			add_redirection(&filtered, current->type, current->filename);
		current = current->next;
	}
	return (filtered);
}

// Exécute le côté droit du pipe avec les redirections filtrées
int	execute_pipe_node_right(t_node *node,\
	char ***envp, int pipefd[2], pid_t pid_left, t_shell *shell)
{
	int		status;
	pid_t	pid_right;
	t_redir	*original_redirections;
	t_redir	*filtered_redirections;

	pid_right = fork();
	if (pid_right == 0)
	{
		// Filtrer les redirections pour ne garder que les sorties
		original_redirections = shell->redirections;
		filtered_redirections = filter_output_redirections\
(original_redirections);
		shell->redirections = filtered_redirections;
		close(pipefd[1]);
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(1);
		}
		close(pipefd[0]);
		exit(execute_node_by_type(node->right, envp, shell));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid_left, NULL, 0);  // Ignorer le statut du process gauche
	waitpid(pid_right, &status, 0);  // Utiliser le statut du dernier process
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

// Exécute un pipe en filtrant les redirections pour chaque côté
int	execute_pipe_node(t_node *node, char ***envp, t_shell *shell)
{
	int		pipefd[2];
	pid_t	pid_left;
	t_redir	*original_redirections;
	t_redir	*filtered_redirections;

	if (pipe(pipefd) == -1)
		return (perror("minishell: pipe"), -1);
	// Sauvegarder les redirections originales
	original_redirections = shell->redirections;
	// Processus gauche (écriture)
	pid_left = fork();
	if (pid_left == 0)
	{
		// Filtrer les redirections pour ne garder que les entrées
		filtered_redirections = filter_input_redirections\
(original_redirections);
		shell->redirections = filtered_redirections;
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		exit(execute_node_by_type(node->left, envp, shell));
	}
	// Restaurer les redirections originales pour le processus parent
	shell->redirections = original_redirections;
	return (execute_pipe_node_right(node, envp, pipefd, pid_left, shell));
}
