/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/17 15:36:30 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/19 16:38:20 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Gestionnaire pour SIGINT (CTRL+C) en mode interactif
void	handle_sigint(int sig)
{
	if (g_signal == SIGNAL_HEREDOC_MODE)
	{
		g_signal = SIGNAL_HEREDOC_INTERRUPTED;
		rl_line_buffer[0] = '\0';
		rl_point = 0;
		rl_end = 0;
		rl_done = 1;
		return ;
	}
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// Configure les signaux pour le mode interactif (shell en attente)
void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

int	donothing(void)
{
	return (0);
}

// Fonctions pour gérer l'état heredoc
void	set_heredoc_mode(int mode)
{
	if (mode)
	{
		g_signal = SIGNAL_HEREDOC_MODE;
		rl_event_hook = donothing;
	}
	else if (g_signal == SIGNAL_HEREDOC_MODE)
		g_signal = SIGNAL_NORMAL;
}
