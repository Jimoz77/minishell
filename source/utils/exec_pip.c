/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pip.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:10:47 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/21 18:11:07 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//a tester
void	execute_pipeline(char **cmds[])
{
    int nb_cmds = 0;
    while (cmds[nb_cmds] != NULL) nb_cmds++; // Compter les commandes

    int prev_pipe_read = -1;
    for (int i = 0; i < nb_cmds; i++) {
        int fd[2];
        if (i < nb_cmds - 1) pipe(fd); // Créer un pipe sauf pour la dernière commande

        pid_t pid = fork();
        if (pid == 0) { // Processus enfant
            // Rediriger stdin depuis le pipe précédent (sauf pour la première commande)
            if (i > 0) {
                dup2(prev_pipe_read, STDIN_FILENO);
                close(prev_pipe_read);
            }
            // Rediriger stdout vers le prochain pipe (sauf pour la dernière commande)
            if (i < nb_cmds - 1) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
            }
            // Exécuter la commande (ex: cmds[i] = {"ls", "-l", NULL})
            execvp(cmds[i][0], cmds[i]);
            perror("execvp"); // Si erreur
            exit(EXIT_FAILURE);
        } else { // Processus parent
            // Fermer les descripteurs inutiles
            if (i > 0) close(prev_pipe_read);
            if (i < nb_cmds - 1) {
                close(fd[1]); // Fermer l'écriture du pipe actuel
                prev_pipe_read = fd[0]; // Sauvegarder la lecture pour la prochaine commande
            }
        }
    }
    // Attendre la fin de tous les enfants
    while (wait(NULL) > 0);
}