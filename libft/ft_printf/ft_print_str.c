/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:20:54 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/30 11:24:20 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_str(const char *str)
{
	int	printed_chars;

	if (!str)
	{
		if (write(1, "(null)", 6) == -1)
			return (-1);
		return (6);
	}
	printed_chars = 0;
	while (str[printed_chars])
	{
		if (ft_print_char(str[printed_chars]) == -1)
			return (-1);
		printed_chars++;
	}
	return (printed_chars);
}
