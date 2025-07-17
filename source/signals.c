/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:09:09 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/17 15:42:39 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	g_signal = 0;

// Gestionnaire pour SIGQUIT (CTRL+\) en mode interactif - ne fait rien
void	handle_sigquit(int sig)
{
	(void)sig;
}

// Configure les signaux pour l'exécution des commandes
void	setup_exec_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Restaure les signaux après l'exécution d'une commande
void	restore_signals(void)
{
	setup_signals();
}

int	is_in_heredoc(void)
{
	return (g_signal == SIGNAL_HEREDOC_MODE);
}
