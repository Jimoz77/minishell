/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_redir_collect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 22:05:42 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/08 19:01:59 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Ajoute une redirection à la liste chaînée
void add_redirection(t_redir **list, t_token_type type, char *filename)
{
    t_redir *new;
    t_redir *tmp;

    new = malloc(sizeof(t_redir));
    if (!new)
        return;

    new->type = type;
    new->filename = ft_strdup(filename);
    if (!new->filename)
    {
        free(new);
        return;
    }
    new->next = NULL;

    if (!*list)
        *list = new;
    else
    {
        tmp = *list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

// Ajoute un heredoc à la liste chaînée
void add_heredoc(t_heredoc **list, char *delimiter, char *content)
{
    t_heredoc *new;
    t_heredoc *tmp;

    new = malloc(sizeof(t_heredoc));
    if (!new)
        return;

    new->delimiter = ft_strdup(delimiter);
    if (!new->delimiter)
    {
        free(new);
        return;
    }
    new->content = content ? ft_strdup(content) : NULL;
    new->processed = 0;
    new->pipe_fd = -1;
    new->next = NULL;

    if (!*list)
        *list = new;
    else
    {
        tmp = *list;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

// Collecte les redirections à partir des tokens
void collect_redirections(t_token *tokens, t_shell *shell)
{
    t_token *current;
    t_token *next;

    shell->redirections = NULL;
    shell->heredocs = NULL;
    
    current = tokens;
    while (current)
    {
        if (is_redirection(current->type) && current->next && 
            current->next->type == TOKEN_WORD)
        {
            next = current->next;
            if (current->type == TOKEN_HEREDOC)
            {
                add_heredoc(&shell->heredocs, next->value, NULL);
            }
            add_redirection(&shell->redirections, current->type, next->value);
        }
        current = current->next;
    }
    
    // Debug des redirections (optionnel)
    debug_redirections(shell);
}

// Fonction pour déboguer les redirections
void debug_redirections(t_shell *shell)
{
    t_redir *current;
    int i;

    current = shell->redirections;
    i = 0;
    ft_printf("\n--- DEBUG REDIRECTIONS ---\n");
    if (!current)
    {
        ft_printf("No redirections found\n");
        ft_printf("-------------------------\n\n");
        return;
    }
    while (current)
    {
        ft_printf("Redirection %d: type=%d ", i, current->type);
        if (current->type == TOKEN_REDIRECT_IN)
            ft_printf("(REDIR_IN)");
        else if (current->type == TOKEN_REDIRECT_OUT)
            ft_printf("(REDIR_OUT)");
        else if (current->type == TOKEN_APPEND)
            ft_printf("(APPEND)");
        else if (current->type == TOKEN_HEREDOC)
            ft_printf("(HEREDOC)");
        ft_printf(" filename='%s'\n", current->filename);
        current = current->next;
        i++;
    }
    ft_printf("-------------------------\n\n");
}
