/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 08:50:08 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:33:16 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
		write (fd, s++, 1);
	write (fd, "\n", 1);
}
/*
#include <fcntl.h> // Pour open

int	main(void)
{
	int	fd;

	// Test 1: Écrire dans la sortie standard (écran)
	ft_putendl_fd("Hello, World!", 1);

	// Test 2: Écrire dans un fichier
	fd = open("test_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		write(1, "Error: Unable to open file\n", 28);
		return (1);
	}
	ft_putendl_fd("This is a test string in the file.", fd);
	close(fd);

	return (0);
}
*/