/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_paren.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 16:12:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/08 19:27:38 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	child_paren_exec(t_node *node, char ***envp, t_shell *shell)
{
	t_redirect	red;

	init_redirect(&red);
	// Les redirections sont maintenant dans le nœud
	if (node->redirections && !apply_node_redirections(node, &red))
	{
		close_redirect_fds(&red);
		restore_std_fds(&red);
		exit(1);
	}
	// Exécuter le contenu des parenthèses
	exit(execute_ast(node->left, envp, shell));
}

int	execute_paren_node(t_node *node, char ***envp, t_shell *shell)
{
    // Si pas de redirections au niveau du nœud parenthèses, 
    // on peut exécuter directement sans fork
    if (!node->redirections)
    {
        return execute_ast(node->left, envp, shell);
    }
    
    // Sinon, utiliser le comportement actuel avec fork
    pid_t	child_pid;
    int		status;

    if (!node || !node->left)
        return (1);
    child_pid = fork();
    if (child_pid == -1)
    {
        perror("minishell: fork");
        return (1);
    }
    if (child_pid == 0)
        child_paren_exec(node, envp, shell);
    waitpid(child_pid, &status, 0);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        return (128 + WTERMSIG(status));
    return (1);
}
