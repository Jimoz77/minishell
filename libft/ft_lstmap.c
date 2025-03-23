/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 15:45:16 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/23 08:59:51 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_elem;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_elem = ft_lstnew(f(lst->content));
		if (!new_elem)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_elem);
		lst = lst->next;
	}
	return (new_list);
}
/*
#include <stdio.h>

// Fonction de transformation pour ft_lstmap : ici, on double une valeur entière
static void	*double_value(void *content)
{
	int *new_value = malloc(sizeof(int));
	if (!new_value)
		return (NULL);
	*new_value = *(int *)content * 2;
	return (new_value);
}

// Fonction de suppression utilisée par ft_lstmap pour libérer la mémoire
static void	del(void *content)
{
	free(content);
}

int	main(void)
{
	// Création des éléments de la liste d'origine
	t_list *list = ft_lstnew(malloc(sizeof(int)));
	*(int *)list->content = 1;

	t_list *second = ft_lstnew(malloc(sizeof(int)));
	*(int *)second->content = 2;
	ft_lstadd_back(&list, second);

	t_list *third = ft_lstnew(malloc(sizeof(int)));
	*(int *)third->content = 3;
	ft_lstadd_back(&list, third);

	// Affichage de la liste d'origine
	printf("Liste d'origine :\n");
	for (t_list *tmp = list; tmp; tmp = tmp->next)
		printf("%d -> ", *(int *)tmp->content);
	printf("NULL\n");

	// Utilisation de ft_lstmap pour créer une nouvelle liste doublee
	t_list *new_list = ft_lstmap(list, double_value, del);

	// Affichage de la nouvelle liste
	printf("Nouvelle liste (valeurs doublées) :\n");
	for (t_list *tmp = new_list; tmp; tmp = tmp->next)
		printf("%d -> ", *(int *)tmp->content);
	printf("NULL\n");

	// Libération de la mémoire des deux listes
	ft_lstclear(&list, del);
	ft_lstclear(&new_list, del);

	return (0);
}
*/