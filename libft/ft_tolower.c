/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:52:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 13:04:32 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (c + 32);
	return (c);
}
/*
#include <stdio.h>

int main(void)
{
    char    test1 = 'Y';
    char    test2 = 'a';
    char    test3 = '!';
    char    test4 = 'R';
    
    printf("%c -> %c\n", test1, ft_tolower(test1));
    printf("%c -> %c\n", test2, ft_tolower(test2));
    printf("%c -> %c\n", test3, ft_tolower(test3));
    printf("%c -> %c\n", test4, ft_tolower(test4));

    return (0);
}
*/