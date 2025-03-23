/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:17:37 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 09:57:42 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
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
			if (ft_isalnum(argv[i][j]))
				printf("%c is alpha or digit\n", argv[i][j]);
			else
				printf("%c is not alpha or digit\n", argv[i][j]);
			j++;
		}
		i++;
	}
	return (0);
}
*/