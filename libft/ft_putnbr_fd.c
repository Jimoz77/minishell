/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:00:56 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:33:22 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return ;
	}
	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		ft_putnbr_fd(n / 10, fd);
	c = (n % 10) + '0';
	write(fd, &c, 1);
}
/*
#include <fcntl.h> // Pour open

int	main(void)
{
	int	fd;

	// Test 1: Écrire un nombre positif
	ft_putnbr_fd(12345, 1);
	write(1, "\n", 1);

	// Test 2: Écrire un nombre négatif
	ft_putnbr_fd(-6789, 1);
	write(1, "\n", 1);

	// Test 3: Écrire zéro
	ft_putnbr_fd(0, 1);
	write(1, "\n", 1);

	// Test 4: Écrire le plus petit entier
	ft_putnbr_fd(-2147483648, 1);
	write(1, "\n", 1);

	// Test 5: Écrire le plus grand entier
	ft_putnbr_fd(2147483647, 1);
	write(1, "\n", 1);

	// Test 6: Écrire dans un fichier
	fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		write(1, "Error: Unable to open file\n", 28);
		return (1);
	}
	ft_putnbr_fd(42, fd);
	write(fd, "\n", 1); // Retour à la ligne
	close(fd);

	return (0);
}
*/