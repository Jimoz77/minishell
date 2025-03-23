/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:05:51 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 09:51:50 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return (1);
	return (0);
}
/*
#include <stdio.h>

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	i = 1;
	(void)argc;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isalpha(argv[i][j]))
				printf("%c is alpha\n", argv[i][j]);
			else
				printf("%c is not alpha\n", argv[i][j]);
			j++;
		}
		i++;
	}
	return (0);
}
*/