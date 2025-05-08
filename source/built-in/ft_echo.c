/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:11:00 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/08 14:55:27 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//ne gere pas encore les {1..10}
// probleme avec les "" mais sera surment réglé dans le parsing
int	ft_echo(char **cmd)
{
	int	line_return;
	int	i;
	int	j;

	line_return = 1;
	i = 1;
	if (!cmd[1])
	{
		printf("\n");
		return (0);
	}
	// Traite les options commençant par "-n" (ex: -n, -nn, -nnn...)
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		// Vérifie que le reste de l'argument ne contient que des 'n' (optionnel)
		j = 2;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] != '\0') // Si autre caractère après "-n", ce n'est PAS une option
			break ;
		line_return = 0;
		i++;
	}
	// Affiche les arguments restants
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		if (cmd[i + 1])
			printf(" ");
		i++;
	}
	if (line_return)
		printf("\n");
	return (0);
}