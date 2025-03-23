/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 20:28:20 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 12:51:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_num_len(long nb)
{
	int	len;

	len = 0;
	if (nb <= 0)
		len++;
	while (nb != 0)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	int		len;

	nb = n;
	len = get_num_len(nb);
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb < 0)
		nb = -nb;
	while (len--)
	{
		str[len] = (nb % 10) + '0';
		nb /= 10;
		if (n < 0 && len == 0)
			str[len] = '-';
	}
	return (str);
}
/*
#include <stdio.h>

int	main(void)
{
	printf("Test 1: %s\n", ft_itoa(123));
	printf("Test 2: %s\n", ft_itoa(-456));
	printf("Test 3: %s\n", ft_itoa(0));
	printf("Test 4: %s\n", ft_itoa(-2147483648));
	printf("Test 5: %s\n", ft_itoa(2147483647));

	return (0);
}
*/