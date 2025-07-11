/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:09:09 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/11 21:48:35 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

// Gestionnaire pour SIGINT (CTRL+C) en mode interactif
void handle_sigint(int sig)
{
    // Si on est dans un heredoc (g_signal == SIGNAL_HEREDOC_MODE)
    if (g_signal == SIGNAL_HEREDOC_MODE)
    {
        g_signal = SIGNAL_HEREDOC_INTERRUPTED;
        write(1, "\n", 1);
        
        // Simuler exactement ce que fait CTRL+D (EOF)
        // En mettant rl_end à 0 et rl_done à 1
        rl_line_buffer[0] = '\0';
        rl_point = 0;
        rl_end = 0;
        rl_done = 1;
        
        return;
    }
    
    // Comportement normal en mode interactif
    g_signal = sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

// Gestionnaire pour SIGQUIT (CTRL+\) en mode interactif - ne fait rien
void handle_sigquit(int sig)
{
    (void)sig;
}

// Configure les signaux pour le mode interactif (shell en attente)
void setup_signals(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;
    
    sa_int.sa_handler = handle_sigint;
    sa_int.sa_flags = 0;  // Pas de SA_RESTART pour permettre l'interruption
    sigemptyset(&sa_int.sa_mask);
    sigaction(SIGINT, &sa_int, NULL);
    
    sa_quit.sa_handler = SIG_IGN;
    sa_quit.sa_flags = 0;
    sigemptyset(&sa_quit.sa_mask);
    sigaction(SIGQUIT, &sa_quit, NULL);
}

// Configure les signaux pour l'exécution des commandes
void setup_exec_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

// Restaure les signaux après l'exécution d'une commande
void restore_signals(void)
{
    setup_signals();
}

// Fonctions pour gérer l'état heredoc
void set_heredoc_mode(int mode)
{
    if (mode)
        g_signal = SIGNAL_HEREDOC_MODE;
    else if (g_signal == SIGNAL_HEREDOC_MODE)
        g_signal = SIGNAL_NORMAL;
}

int is_in_heredoc(void)
{
    return (g_signal == SIGNAL_HEREDOC_MODE);
}
