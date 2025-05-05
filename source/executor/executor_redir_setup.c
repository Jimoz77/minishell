/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_setup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 19:42:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Collecte les redirections dans l'ordre de traitement */
int	collect_redirects(t_node *node, t_node **redirections)
{
	t_node	*stack[100];
	int		top;
	int		i;

	top = 0;
	while (node && is_redirect_node(node->type))
	{
		stack[top++] = node;
		node = node->left;
	}
	i = 0;
	while (top > 0)
	{
		redirections[i++] = stack[--top];
	}
	return (i);
}

/* Traite une redirection et met à jour les descripteurs */
int	process_redirect_node(t_node *node, t_redirect *red, int *in, int *out)
{
	if (node->type == NODE_REDIRECT_IN)
	{
		if (red->stdin_fd != -1)
			close(red->stdin_fd);
		if (!process_redirect_in(node, red))
			return (0);
		*in = 1;
	}
	else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_APPEND)
	{
		if (red->stdout_fd != -1)
			close(red->stdout_fd);
		if (!process_redirect_out(node, red))
			return (0);
		*out = 1;
	}
	return (1);
}

/* Applique les redirections avec dup2 sur les descripteurs */
int	apply_redirections(int in_redir, int out_redir, t_redirect *red)
{
	if (in_redir && red->stdin_fd != -1)
	{
		if (dup2(red->stdin_fd, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 stdin");
			return (0);
		}
	}
	if (out_redir && red->stdout_fd != -1)
	{
		if (dup2(red->stdout_fd, STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 stdout");
			return (0);
		}
	}
	return (1);
}

/* Configure et applique toutes les redirections */
int	setup_all_redirects(t_node *node, t_redirect *red, t_shell *shell)
{
	t_node	*redirections[100];
	int		count;
	int		i;
	int		in_redir;
	int		out_redir;

	count = collect_redirects(node, redirections);
	i = 0;
	in_redir = 0;
	out_redir = 0;
	
	// Traiter toutes les redirections dans l'ordre
	while (i < count)
	{
		if (redirections[i]->type == NODE_HEREDOC)
		{
			if (red->stdin_fd != -1)
				close(red->stdin_fd);
			if (!process_heredoc_node(redirections[i], red, shell))
				return (0);
			in_redir = 1;
		}
		else if (redirections[i]->type == NODE_REDIRECT_IN)
		{
			if (red->stdin_fd != -1)
				close(red->stdin_fd);
			if (!process_redirect_in(redirections[i], red))
				return (0);
			in_redir = 1;
		}
		else if (redirections[i]->type == NODE_REDIRECT_OUT || 
				redirections[i]->type == NODE_APPEND)
		{
			if (red->stdout_fd != -1)
				close(red->stdout_fd);
			if (!process_redirect_out(redirections[i], red))
				return (0);
			out_redir = 1;
		}
		i++;
	}
	
	return (apply_redirections(in_redir, out_redir, red));
}
