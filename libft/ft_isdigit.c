/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:14:31 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 10:02:12 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
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
			if (ft_isdigit(argv[i][j]))
				printf("%c is digit\n", argv[i][j]);
			else 
				printf("%c is not digit\n", argv[i][j]);
			j++;
		}
		i++;
	}
	return (0);
}
*/