/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 08:25:38 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:33:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
/*
#include <stdio.h>
#include <fcntl.h> // Pour utiliser open

int	main(void)
{
	int	fd;

	// Test 1: Ecrire dans la sortie standard (écran)
	ft_putchar_fd('A', 1);
	write(1, "\n", 1); // Nouvelle ligne pour plus de clarté

	// Test 2: Ecrire dans un fichier
	fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("Error opening file");
		return (1);
	}
	ft_putchar_fd('B', fd);
	ft_putchar_fd('\n', fd);
	close(fd);

	return (0);
}
*/