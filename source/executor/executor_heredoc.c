/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:40:45 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 20:56:23 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Convertit une chaîne en descripteur de fichier via un pipe
int string_to_fd(const char *content)
{
    int pipe_fd[2];

    if (!content)
        return (-1);
    if (pipe(pipe_fd) == -1)
    {
        perror("minishell: pipe");
        return (-1);
    }
    if (write(pipe_fd[1], content, ft_strlen(content)) == -1)
    {
        perror("minishell: write");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (-1);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}

// Lit l'entrée jusqu'au délimiteur et renvoie le contenu
static char *read_heredoc_input(const char *delimiter)
{
    char *line;
    char *content;
    char *new_content;

    content = ft_strdup("");
    if (!content)
        return (NULL);
    
    while (1)
    {
        // Utiliser directement readline avec une invite
        line = readline("> ");
        
        // Vérifier si c'est EOF ou le délimiteur
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
        {
            if (line)
                free(line);
            break;
        }
        
        // Concaténer la ligne au contenu
        new_content = ft_strjoin(content, line);
        free(content);
        free(line);
        
        // Ajouter un saut de ligne après chaque ligne
        content = ft_strjoin(new_content, "\n");
        free(new_content);
        
        if (!content)
            return (NULL);
    }
    return (content);
}

// Traite tous les heredocs dans l'ordre
void process_heredocs(t_shell *shell)
{
    t_heredoc *current;

    current = shell->heredocs;
    while (current)
    {
        if (!current->processed && current->delimiter)
        {
            // Lecture des lignes jusqu'au délimiteur
            current->content = read_heredoc_input(current->delimiter);
            
            // Création du pipe pour stocker le contenu
            if (current->content)
            {
                current->pipe_fd = string_to_fd(current->content);
                if (current->pipe_fd == -1)
                {
                    perror("minishell: heredoc pipe");
                    free(current->content);
                    current->content = NULL;
                }
            }
            current->processed = 1;
        }
        current = current->next;
    }
}

// Affiche l'état actuel des heredocs (debug)
void debug_heredocs(t_shell *shell)
{
    t_heredoc *current;
    int i;

    current = shell->heredocs;
    i = 0;
    ft_printf("\n--- DEBUG HEREDOCS ---\n");
    if (!current)
    {
        ft_printf("No heredocs found\n");
        ft_printf("---------------------\n\n");
        return;
    }
    while (current)
    {
        ft_printf("Heredoc %d: delimiter='%s', processed=%d, pipe_fd=%d\n", 
                 i, current->delimiter ? current->delimiter : "(null)", 
                 current->processed, current->pipe_fd);
        if (current->content)
            ft_printf("Content (%zu bytes)\n", ft_strlen(current->content));
        else
            ft_printf("Content: NULL\n");
        current = current->next;
        i++;
    }
    ft_printf("---------------------\n\n");
}
