/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 10:32:43 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 10:13:28 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
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
            if (ft_isprint(argv[i][j]))
                printf("%c is printable\n", argv[i][j]);
            else
                printf("%c is not printable\n", argv[i][j]);
            j++;
        }
        i++;
    }
    return (0);
}
*/