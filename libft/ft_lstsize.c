/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:29:50 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 10:27:02 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}
/*
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_list *list = NULL;

	// Ajout de quelques éléments à la liste
	ft_lstadd_front(&list, ft_lstnew("Element 1"));
	ft_lstadd_front(&list, ft_lstnew("Element 2"));
	ft_lstadd_front(&list, ft_lstnew("Element 3"));

	// Compter le nombre d'éléments
	int size = ft_lstsize(list);
	printf("Size of the list: %d\n", size); // Devrait afficher 3

	// Libérer la mémoire
	ft_lstdelone(list->next->next, free); // Libérer "Element 1"
	ft_lstdelone(list->next, free); // Libérer "Element 2"
	ft_lstdelone(list, free); // Libérer "Element 3"
	return (0);
}
*/