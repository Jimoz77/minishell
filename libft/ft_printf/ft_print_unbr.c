/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 19:50:12 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/11/06 11:05:21 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_unbr(unsigned int n)
{
	int	printed_chars;

	printed_chars = 0;
	if (n >= 10)
	{
		printed_chars = ft_print_unbr(n / 10);
		if (printed_chars == -1)
			return (-1);
	}
	if (ft_print_char((n % 10) + '0') == -1)
		return (-1);
	printed_chars++;
	return (printed_chars);
}
