/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 11:38:34 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/16 09:15:21 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void *))
{
	if (!lst)
		return ;
	if (del)
		del(lst->content);
	free (lst);
}
/*
static void	del(void *content)
{
	free(content);
}

#include "stdio.h"

int	main(void)
{
	// Création d'un maillon de liste
	t_list *node = ft_lstnew(malloc(sizeof(char) * 20)); // Exemple de contenu
	if (!node)
		return (1); // Erreur d'allocation

	node->content = malloc(20); // Exemple de contenu
	sprintf(node->content, "Hello, World!");

	printf("Before deletion: %s\n", (char *)node->content); // Affiche le contenu

	ft_lstdelone(node, del); // Supprime le maillon de la liste

	return (0);
}
*/