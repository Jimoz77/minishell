/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:10:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/18 13:02:03 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Collecte tous les nœuds de redirection dans un tableau
int	collect_redirects(t_node *node, t_node **redirections)
{
	t_node	*current;
	int		count;

	current = node;
	count = 0;
	while (current && is_redirect_node(current->type))
	{
		redirections[count++] = current;
		if (current->left && is_redirect_node(current->left->type))
			current = current->left;
		else
			break ;
	}
	return (count);
}

// Traite un nœud heredoc et stocke son fd dans la structure redirect
int	process_heredoc_node(t_node *current, t_redirect *red)
{
	int	heredoc_fd;

	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
	heredoc_fd = process_single_heredoc(current);
	if (heredoc_fd == -1)
		return (0);
	red->stdin_fd = heredoc_fd;
	return (1);
}

// Traite une redirection d'entrée (<) et ouvre le fichier
int	process_redirect_in(t_node *current, t_redirect *red)
{
	char	*filename;

	if (red->stdin_fd != -1)
	{
		close(red->stdin_fd);
		red->stdin_fd = -1;
	}
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

// Traite une redirection de sortie (> ou >>) et ouvre le fichier
int	process_redirect_out(t_node *current, t_redirect *red)
{
	int		flags;
	char	*filename;

	if (red->stdout_fd != -1)
	{
		close(red->stdout_fd);
		red->stdout_fd = -1;
	}
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
		if (red->stdin_fd != -1)
			close(red->stdin_fd);
		return (0);
	}
	return (1);
}

// Traite un nœud de redirection selon son type et met à jour les flags
int	process_redirect_node(t_node *node, t_redirect *red, int *in, int *out)
{
	if (node->type == NODE_HEREDOC && !process_heredoc_node(node, red))
		return (0);
	if (node->type == NODE_REDIRECT_IN && !process_redirect_in(node, red))
		return (0);
	if ((node->type == NODE_REDIRECT_OUT || node->type == NODE_APPEND)
		&& !process_redirect_out(node, red))
		return (0);
	if (node->type == NODE_HEREDOC || node->type == NODE_REDIRECT_IN)
		*in = 1;
	if (node->type == NODE_REDIRECT_OUT || node->type == NODE_APPEND)
		*out = 1;
	return (1);
}
