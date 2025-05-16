/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:11:00 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/14 20:40:51 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_echo2(char **cmd, int i, int line_return)
{
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
	while (cmd[i] && cmd[i][0] == '-' && cmd[i][1] == 'n')
	{
		j = 2;
		while (cmd[i][j] == 'n')
			j++;
		if (cmd[i][j] != '\0')
			break ;
		line_return = 0;
		i++;
	}
	return (ft_echo2(cmd, i, line_return));
}
