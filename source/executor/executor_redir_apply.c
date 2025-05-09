/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_apply.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:20:28 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/09 14:06:55 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Applique un heredoc en utilisant son pipe_fd
static int apply_heredoc_redir(t_shell *shell, t_redir *redir, t_redirect *red)
{
    t_heredoc *heredoc;

    // Fermer le précédent fd d'entrée s'il existe
    if (red->stdin_fd != -1)
    {
        close(red->stdin_fd);
        red->stdin_fd = -1;
    }
    
    // Rechercher le heredoc correspondant
    heredoc = shell->heredocs;
    while (heredoc)
    {
        if (heredoc->delimiter && ft_strncmp(heredoc->delimiter, redir->filename, 
                                          ft_strlen(redir->filename) + 1) == 0 && 
            heredoc->processed && heredoc->pipe_fd != -1)
        {
            // Dupliquer le descripteur pour éviter les problèmes de fermeture
            red->stdin_fd = dup(heredoc->pipe_fd);
            if (red->stdin_fd == -1)
            {
                perror("minishell: dup");
                return (0);
            }
            return (1);
        }
        heredoc = heredoc->next;
    }
    
    ft_putstr_fd("minishell: warning: heredoc missing\n", 2);
    return (0);
}

// Affiche un message d'erreur pour un fichier
static void print_file_error(char *filename, int error_code)
{
    ft_putstr_fd("minishell: ", 2);
    ft_putstr_fd(filename, 2);
    ft_putstr_fd(": ", 2);
    
    if (error_code == EISDIR)
        ft_putendl_fd("Is a directory", 2);
    else if (error_code == EACCES)
        ft_putendl_fd("Permission denied", 2);
    else if (error_code == ENOENT)
        ft_putendl_fd("No such file or directory", 2);
    else
        ft_putendl_fd(strerror(error_code), 2);
}

// Applique une redirection entrante "<"
static int apply_redirect_in(t_redir *redir, t_redirect *red)
{
    // Fermer le précédent fd d'entrée s'il existe
    if (red->stdin_fd != -1)
    {
        close(red->stdin_fd);
        red->stdin_fd = -1;
    }
    
    // Ouvrir le fichier en lecture
    red->stdin_fd = open(redir->filename, O_RDONLY);
    if (red->stdin_fd == -1)
    {
        // Message d'erreur conforme à bash
        print_file_error(redir->filename, errno);
        return (0);
    }
    return (1);
}

// Applique une redirection sortante ">" ou ">>"
static int apply_redirect_out(t_redir *redir, t_redirect *red)
{
    int flags;
    
    // Fermer le précédent fd de sortie s'il existe
    if (red->stdout_fd != -1)
    {
        close(red->stdout_fd);
        red->stdout_fd = -1;
    }
    
    // Définir les flags en fonction du type de redirection
    flags = O_WRONLY | O_CREAT;
    if (redir->type == TOKEN_REDIRECT_OUT)
        flags |= O_TRUNC;
    else
        flags |= O_APPEND;
    
    // Ouvrir le fichier en écriture
    red->stdout_fd = open(redir->filename, flags, 0644);
    if (red->stdout_fd == -1)
    {
        // Message d'erreur conforme à bash
        print_file_error(redir->filename, errno);
        return (0);
    }
    return (1);
}

// Applique toutes les redirections et effectue les dup2 nécessaires
int apply_all_redirections(t_shell *shell, t_redirect *red)
{
    t_redir *current;
    int success;
    
    current = shell->redirections;
    while (current)
    {
        success = 1;
        if (current->type == TOKEN_REDIRECT_IN)
            success = apply_redirect_in(current, red);
        else if (current->type == TOKEN_REDIRECT_OUT || current->type == TOKEN_APPEND)
            success = apply_redirect_out(current, red);
        else if (current->type == TOKEN_HEREDOC)
            success = apply_heredoc_redir(shell, current, red);
        
        if (!success)
            return (0);
        
        current = current->next;
    }
    
    // Effectuer les redirections finales avec dup2
    if (red->stdin_fd != -1)
    {
        if (dup2(red->stdin_fd, STDIN_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (0);
        }
    }
    
    if (red->stdout_fd != -1)
    {
        if (dup2(red->stdout_fd, STDOUT_FILENO) == -1)
        {
            perror("minishell: dup2");
            return (0);
        }
    }
    
    return (1);
}
