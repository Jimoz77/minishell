/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:40:45 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/17 15:56:55 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*read_heredoc_input_expanded(const char *delimiter,
									t_shell *shell)
{
    char *content;
    size_t delim_len;
    int result;
    
    content = ft_strdup("");
    if (!content)
        return (NULL);
    delim_len = ft_strlen(delimiter);
    while (1)
    {
        result = process_heredoc_line(delimiter, shell, &content, delim_len);
        if (result == 0)
			break ;
        if (result == -1)
        {
            free(content);
            return (NULL);
        }
    }
    return (content);
}

static void process_node_heredocs_expanded(t_node *node, t_shell *shell)
{
    t_heredoc *heredoc;
    
    if (!node || !node->heredocs)
        return ;
    heredoc = node->heredocs;
    while (heredoc)
    {
        if (!heredoc->processed && heredoc->delimiter)
        {
            heredoc->content = read_heredoc_input_expanded(heredoc->delimiter, shell);
            
            // Vérifier l'interruption APRÈS l'appel
            if (shell->exit_status == 130)
            {
                // NE PAS marquer comme traité, juste nettoyer et sortir
                if (heredoc->content)
                {
                    free(heredoc->content);
                    heredoc->content = NULL;
                }
                // NE PAS FAIRE heredoc->processed = 1;
                return;  // Sortir immédiatement
            }
            
            if (heredoc->content)
            {
                heredoc->pipe_fd = string_to_fd(heredoc->content);
                if (heredoc->pipe_fd == -1)
                {
                    free(heredoc->content);
                    heredoc->content = NULL;
                }
            }
            heredoc->processed = 1;
        }
        heredoc = heredoc->next;
    }
}

void process_all_heredocs_expanded(t_node *node, t_shell *shell)
{
    if (!node)
        return ;
    
    process_node_heredocs_expanded(node, shell);
    if (shell->exit_status == 130)
        return; // Sortir immédiatement si heredoc interrompu
    
    process_all_heredocs_expanded(node->left, shell);
    if (shell->exit_status == 130)
        return; // Sortir immédiatement si heredoc interrompu
    
    process_all_heredocs_expanded(node->right, shell);
}
