/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:43:35 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/31 12:21:09 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_ptr(void *ptr)
{
	int	printed_chars;
	int	current_printed;

	printed_chars = 0;
	if (!ptr)
	{
		if (write(1, "0x0", 3) == -1)
			return (-1);
		return (3);
	}
	if (write(1, "0x", 2) == -1)
		return (-1);
	printed_chars += 2;
	current_printed = ft_print_hex((unsigned long)ptr, 'x');
	if (current_printed == -1)
		return (-1);
	printed_chars += current_printed;
	return (printed_chars);
}
