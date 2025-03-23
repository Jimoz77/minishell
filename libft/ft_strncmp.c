/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:22:11 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:34:11 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	while (i < n && (s1[i] && s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] -(unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
/*
#include <stdio.h>

int main(void)
{
    const char *s1 = "Hello";
    const char *s2 = "Helium";

    int result = ft_strncmp(s1, s2, 3);
    if (result == 0)
        printf("The 3 first character are egual\n");
    else if (result < 0)
        printf("The first string is smaller\n");
    else
        printf("The first string is bigger\n");
    
    return (0);
}
*/