/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:42:53 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/16 09:05:40 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	last = *lst;
	while (last->next)
		last = last->next;
	last->next = new;
}
/*
#include <stdio.h>

int	main(void)
{
	t_list *list = NULL;

	// Ajout d'un élément au début
	ft_lstadd_front(&list, ft_lstnew("First Element"));

	// Ajout d'un élément à la fin
	ft_lstadd_back(&list, ft_lstnew("Second Element"));

	// Ajout d'un autre élément à la fin
	ft_lstadd_back(&list, ft_lstnew("Third Element"));

	// Affichage de la liste
	t_list *current = list;
	while (current)
	{
		printf("Content: %s\n", (char *)current->content);
		current = current->next; // Passer au maillon suivant
	}

	// Libérer la mémoire
	ft_lstdelone(list->next->next, free); // Libérer "Third Element"
	ft_lstdelone(list->next, free); // Libérer "Second Element"
	ft_lstdelone(list, free); // Libérer "First Element"

	return (0);
}
*/