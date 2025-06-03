/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:39:05 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/03 13:24:19 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//pour l'instant print uniquement l'env
//a voir plus tard si besoin de return l'env
//la commande env avec un argument print la env_var spécifié ex : env $PATH
int ft_env(char ***envp) {
    int i = 0;
    while ((*envp)[i]) {
        // Vérifier si stdout est toujours valide
        if (write(STDOUT_FILENO, (*envp)[i], strlen((*envp)[i])) == -1) {
            break;  // Arrêter si écriture impossible
        }
        write(STDOUT_FILENO, "\n", 1);  // Écrire manuellement le saut de ligne
        i++;
    }
    return 1;
}
