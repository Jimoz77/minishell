/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:10:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 18:58:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Applique un heredoc en écrasant red->stdin_fd si valide */
int	process_heredoc_node(t_node *current, t_redirect *red, t_shell *shell)
{
	if (red->stdin_fd != -1)
		close(red->stdin_fd);
	if (!shell->heredocs || current->heredoc_index < 0)
		return (0);
	red->stdin_fd = string_to_fd(shell->heredocs[current->heredoc_index]);
	if (red->stdin_fd == -1)
		return (0);
	return (1);
}

// Ouvre un fichier en lecture pour une redirection "<"
int	process_redirect_in(t_node *current, t_redirect *red)
{
	char	*filename;

	if (red->stdin_fd != -1)
		close(red->stdin_fd);
	filename = get_unquoted_filename(current);
	if (!filename)
		return (0);
	red->stdin_fd = open(filename, O_RDONLY);
	free(filename);
	if (red->stdin_fd == -1)
	{
		perror("minishell");
		return (0);
	}
	return (1);
}

// Ouvre un fichier en écriture pour ">" ou ">>"
int	process_redirect_out(t_node *current, t_redirect *red)
{
	int		flags;
	char	*filename;

	if (red->stdout_fd != -1)
		close(red->stdout_fd);
	flags = O_WRONLY | O_CREAT;
	if (current->type == NODE_REDIRECT_OUT)
		flags |= O_TRUNC;
	else
		flags |= O_APPEND;
	filename = get_unquoted_filename(current);
	if (!filename)
		return (0);
	red->stdout_fd = open(filename, flags, 0644);
	free(filename);
	if (red->stdout_fd == -1)
	{
		perror("minishell");
		return (0);
	}
	return (1);
}

// Recherche récursivement la commande dans tout l'arbre
t_node	*find_command_node(t_node *node)
{
	t_node	*left;
	t_node	*right;

	if (!node)
		return (NULL);
	if (node->type == NODE_CMD)
		return (node);
	left = find_command_node(node->left);
	if (left)
		return (left);
	right = find_command_node(node->right);
	if (right)
		return (right);
	return (NULL);
}
