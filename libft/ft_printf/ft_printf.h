/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch > +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 15:24:13 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/29 09:05:41 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>

int	ft_printf(const char *format, ...);
int	ft_print_char(char c);
int	ft_print_str(const char *str);
int	ft_print_nbr(int n);
int	ft_print_ptr(void *ptr);
int	ft_print_unbr(unsigned int n);
int	ft_print_hex(unsigned long n, char format);

#endif