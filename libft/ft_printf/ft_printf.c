/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:35:04 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/11/06 09:20:03 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	check_format(char format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	if (format == 'c')
		printed_chars += ft_print_char((char)va_arg(args, int));
	else if (format == 's')
		printed_chars += ft_print_str(va_arg(args, char *));
	else if (format == 'p')
		printed_chars += ft_print_ptr(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		printed_chars += ft_print_nbr(va_arg(args, int));
	else if (format == 'u')
		printed_chars += ft_print_unbr(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		printed_chars += ft_print_hex(va_arg(args, unsigned int), format);
	else if (format == '%')
		printed_chars += ft_print_char('%');
	return (printed_chars);
}

static int	handle_format(const char **format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	if (**format == '%' && *(*format + 1))
	{
		(*format)++;
		printed_chars = check_format(**format, args);
	}
	else
	{
		if (ft_print_char(**format) == -1)
			return (-1);
		printed_chars = 1;
	}
	(*format)++;
	return (printed_chars);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		printed_chars;
	int		current_printed;

	printed_chars = 0;
	va_start(args, format);
	while (*format)
	{
		current_printed = handle_format(&format, args);
		if (current_printed == -1)
		{
			va_end(args);
			return (-1);
		}
		printed_chars += current_printed;
	}
	va_end(args);
	return (printed_chars);
}
