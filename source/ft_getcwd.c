/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getcwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:33:44 by jiparcer          #+#    #+#             */
/*   Updated: 2025/05/14 21:34:45 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// donne le path actuel a cwd

void	ft_getcwd(void)
{
	char	cwd[BUFSIZ];

	getcwd(cwd, sizeof(cwd));
	printf("\033[34m[~%s]\033[0m\n", cwd);
}
