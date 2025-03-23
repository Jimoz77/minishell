/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 10:36:36 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 15:31:04 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = malloc(count * size);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
/*
#include <stdio.h>

int main(void)
{
    size_t count = 5;  // Number of elements to allocate
    size_t size = sizeof(int);  // Size of each element

    // Call to ft_calloc
    int *arr = (int *)ft_calloc(count, size);

    // Check if the allocation was successful
    if (!arr)  // Check for allocation failure
    {
        printf("Allocation failed\n");
        return (1);
    }

    // Print values in the allocated array
    printf("Allocated memory initialized to zero:\n");
    
    size_t i = 0;
    while (i < count)
    {
        printf("%d ", arr[i]); // Should print 0 for all elements
        i++;
    }
    printf("\n");

    // Free the allocated memory
    free(arr);

    return 0;
}
*/