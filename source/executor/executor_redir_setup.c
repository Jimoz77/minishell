/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_setup.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:20:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/04/17 11:37:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Applique les redirections finales aux descripteurs standard
static int	apply_redirections(int in_redir, int out_redir, t_redirect *red)
{
	if (in_redir)
		dup2(red->stdin_fd, STDIN_FILENO);
	if (out_redir)
		dup2(red->stdout_fd, STDOUT_FILENO);
	return (1);
}

// Configure toutes les redirections pour un nœud
int	setup_all_redirects(t_node *node, t_redirect *red)
{
	t_node	*redirections[100];
	int		count;
	int		i;
	int		in_redir;
	int		out_redir;

	in_redir = 0;
	out_redir = 0;
	count = collect_redirects(node, redirections);
	i = count - 1;
	while (i >= 0)
	{
		if (!process_redirect_node(redirections[i], red, &in_redir, &out_redir))
			return (0);
		i--;
	}
	return (apply_redirections(in_redir, out_redir, red));
}
