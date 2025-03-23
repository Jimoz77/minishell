/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:53:53 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 12:11:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}
/*
#include <stdio.h>

int main(int argc, char **argv)
{
    int i;
    int j;

    i = 1;
    (void)argc;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (ft_isascii(argv[i][j]))
                printf ("%c is ascii\n", argv[i][j]);
            else
                printf ("%c is not ascii\n", argv[i][j]);
            j++;
        }
        i++;
    }
    return (0);
}
*/