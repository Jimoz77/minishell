/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:39:05 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 15:26:30 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//pour l'instant print uniquement l'env
//a voir plus tard si besoin de return l'env
//la commande env avec un argument print la env_var spécifié ex : env $PATH
int	ft_env(char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		if (write(STDOUT_FILENO, (*envp)[i], strlen((*envp)[i])) == -1)
			break ;
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (1);
}
