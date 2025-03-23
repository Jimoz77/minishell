/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:12:23 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 12:29:58 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
		return (0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
/*
#include <stdio.h>

int main(void)
{
    const char *num_str1 = "   -12345";
    const char *num_str2 = "42";
    const char *num_str3 = "    +6789abc";
    const char *num_str4 = "  \t   \t 0";

    printf("ft_atoi(\"%s\") = %d\n", num_str1, ft_atoi(num_str1));
    printf("ft_atoi(\"%s\") = %d\n", num_str2, ft_atoi(num_str2));
    printf("ft_atoi(\"%s\") = %d\n", num_str3, ft_atoi(num_str3));
    printf("ft_atoi(\"%s\") = %d\n", num_str4, ft_atoi(num_str4));

    return 0;
}
*/