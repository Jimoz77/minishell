/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 20:16:28 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/31 12:20:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_hex(unsigned long n, char format)
{
	char	*base;
	int		printed_chars;
	int		current_printed;

	printed_chars = 0;
	if (format == 'x')
		base = "0123456789abcdef";
	else
		base = "0123456789ABCDEF";
	if (n >= 16)
	{
		current_printed = ft_print_hex(n / 16, format);
		if (current_printed == -1)
			return (-1);
		printed_chars += current_printed;
	}
	if (ft_print_char(base[n % 16]) == -1)
		return (-1);
	printed_chars++;
	return (printed_chars);
}
