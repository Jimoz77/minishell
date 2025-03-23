/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:07:18 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/16 09:15:19 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*temp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
/*
static void	del(void *content)
{
	free(content);
}

#include <stdio.h>

int	main(void)
{
	t_list *head = ft_lstnew(malloc(10));
	t_list *second = ft_lstnew(malloc(20));

	ft_lstadd_back(&head, second); // Ajoute le deuxième élément à la liste

	// Test avant suppression
	printf("Before clear: head = %p, second = %p\n", (void *)head, (void *)second);

	ft_lstclear(&head, del); // Appelle la fonction pour tout supprimer

	// Test après suppression
	printf("After clear: head = %p\n", (void *)head); // Devrait afficher NULL

	return (0);
}
*/