/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 10:13:37 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 09:57:29 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
}
/*	
#include <stdio.h>
#include <stdlib.h>

int	main(void)
{
	t_list *head = ft_lstnew("First Element");
	t_list *second = ft_lstnew("Second Element");

	ft_lstadd_front(&head, second); // Ajoute "Second Element" en tête

	// Affichage de la liste
	t_list *temp = head;
	while (temp != NULL)
	{
		printf("Content: %s\n", (char *)temp->content);
		temp = temp->next;
	}

	// Libération de la mémoire
	ft_lstdelone(head, free); // Libère le premier élément
	ft_lstdelone(second, free); // Libère le second élément

	return (0);
}
*/