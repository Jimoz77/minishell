/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_combined.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 12:28:14 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/22 16:33:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Sauvegarde les descripteurs standard et initialise la structure
static void	init_redirects(t_redirect *red)
{
	red->stdin_fd = -1;
	red->stdout_fd = -1;
	red->saved_stdin = dup(STDIN_FILENO);
	red->saved_stdout = dup(STDOUT_FILENO);
}

// Restaure les descripteurs standard et ferme les descripteurs ouverts
static void	restore_redirects(t_redirect *red)
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
	dup2(red->saved_stdin, STDIN_FILENO);
	dup2(red->saved_stdout, STDOUT_FILENO);
	close(red->saved_stdin);
	close(red->saved_stdout);
}

// Trouve la commande (feuille de l'arbre)
static t_node	*find_command_node(t_node *node)
{
	while (node && node->type != NODE_CMD)
	{
		if (is_redirect_node(node->type))
			node = node->left;
		else
			break ;
	}
	return (node);
}

// Exécution dans le processus enfant
static void execute_child(t_node *node, char **envp, t_redirect *red)
{
	t_node	*cmd_node;

	// Configurer toutes les redirections
	if (!setup_all_redirects(node, red))
		exit(1);
	// Trouver le nœud de commande (le dernier nœud à gauche)
	cmd_node = find_command_node(node);

	// Exécuter la commande si elle existe
	if (cmd_node && cmd_node->type == NODE_CMD)
		exit(execute_cmd_node(cmd_node, envp));
	else
		exit(1); // Erreur: pas de commande trouvée
}

// Execute une commande avec toutes ses redirections
int	execute_combined_node(t_node *node, char ***envp)
{
	t_redirect	red;
	int 		status;
	pid_t		pid;

	init_redirects(&red);
	pid = fork();
	if (pid == 0)
	{
		// Restaurer les gestionnaires de signaux par défaut dans le processus fils
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child(node, envp, &red);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		restore_redirects(&red);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
	}
	else
	{
		perror("minishell: fork");
		restore_redirects(&red);
	}
	return (1);
}
