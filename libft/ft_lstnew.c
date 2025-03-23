/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 09:58:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2024/10/10 09:12:47 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}
/*
#include <stdio.h>

int	main(void)
{
	t_list	*node = ft_lstnew("Hello, World!");

	if (node)
	{
		printf("Content: %s\n", (char *)node->content);
		printf("Next: %p\n", (void *)node->next);
	}
	else
	{
		printf("Memory allocation failed.\n");
	}

	free(node); // N'oublie pas de libérer la mémoire
	return (0);
}
*/