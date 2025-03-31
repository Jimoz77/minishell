/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:11:00 by jimpa             #+#    #+#             */
/*   Updated: 2025/03/31 19:43:42 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//ne gere pas encore les {1..10}
// probleme avec les "" mais sera surment réglé dans le parsing
int	ft_echo(char **cmd)
{
	int	line_return;
	int	i;

	line_return = 1;
	i = 1;
	if (cmd[1][0] == '-' && cmd[1][1] == 'n' && cmd[1][2] == '\0')
	{
		line_return = 0;
		i = 2;
	}
	while (cmd[i])
	{
		printf("%s", cmd[i]);
		i++;
		if	(cmd[i])
			printf(" ");
	}
	if (line_return == 1)
		printf("\n");
	return (1);
}
