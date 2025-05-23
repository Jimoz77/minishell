/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:09:09 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/22 16:06:55 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

// Gestionnaire pour SIGINT (CTRL+C), affiche un new prompt sur une new ligne
void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

// Gestionnaire pour SIGQUIT (CTRL+\), ne fais rien en mode interactif

// Configure le gestionnaire pour SIGINT
static void	setup_sigint(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = handle_sigint;
	sa_int.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);
}

// Configure le gestionnaire pour SIGQUIT
static void	setup_sigquit(void)
{
	struct sigaction	sa_quit;

	sa_quit.sa_handler = SIG_IGN;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_quit.sa_mask);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Configure tous les gestionnaires de signaux pour le shell
void	setup_signals(void)
{
	setup_sigint();
	setup_sigquit();
}
