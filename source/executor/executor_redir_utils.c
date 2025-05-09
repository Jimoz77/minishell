/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:10:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 18:51:16 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Initialise la structure de redirection
void init_redirect(t_redirect *red)
{
    red->stdin_fd = -1;
    red->stdout_fd = -1;
    red->saved_stdin = dup(STDIN_FILENO);
    red->saved_stdout = dup(STDOUT_FILENO);
}

// Ferme les descripteurs ouverts dans les redirections
void close_redirect_fds(t_redirect *red)
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
}

// Restaure les descripteurs standard à leurs valeurs originales
void restore_std_fds(t_redirect *red)
{
    if (red->saved_stdin != -1)
    {
        dup2(red->saved_stdin, STDIN_FILENO);
        close(red->saved_stdin);
        red->saved_stdin = -1;
    }
    if (red->saved_stdout != -1)
    {
        dup2(red->saved_stdout, STDOUT_FILENO);
        close(red->saved_stdout);
        red->saved_stdout = -1;
    }
}

// Recherche récursivement la commande dans tout l'arbre
t_node *find_command_node(t_node *node)
{
    t_node *left;
    t_node *right;

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

// Vérifie si le type correspond à une redirection
int is_redirect_node(t_node_type type)
{
    return (type == NODE_REDIRECT_IN || type == NODE_REDIRECT_OUT
        || type == NODE_APPEND || type == NODE_HEREDOC);
}
