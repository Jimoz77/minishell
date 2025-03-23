/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 14:36:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/08 13:04:30 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
		return (c - 32);
	return (c);
}
/*
#include <stdio.h>

int main(void)
{
	char	test1 = 'y';
	char	test2 = 'P';
	char	test3 = '3';
	char	test4 = 'a';

	printf("%c -> %c\n", test1, ft_toupper(test1));
	printf("%c -> %c\n", test2, ft_toupper(test2));
	printf("%c -> %c\n", test3, ft_toupper(test3));
	printf("%c -> %c\n", test4, ft_toupper(test4));

    return (0);
}
*/