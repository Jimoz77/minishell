/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:36:07 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 10:40:43 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
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

	// Trouver le dernier élément
	t_list *last = ft_lstlast(list);
	if (last)
		printf("Last element: %s\n", (char *)last->content);

	// Libérer la mémoire
	ft_lstdelone(list->next->next, free); // Libérer "Element 1"
	ft_lstdelone(list->next, free); // Libérer "Element 2"
	ft_lstdelone(list, free); // Libérer "Element 3"
	return (0);
}
*/