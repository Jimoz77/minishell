/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:59:25 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/21 16:46:38 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char **my_envp;

	//my_envp =ft_array_dup(envp);

	//fonctionne maintenant avec son propre fichier d'env
	//a mettre a jour avec ordi de l ecole
	// + gerer si suppression du fichier
	my_envp = load_env();
	ft_read_line(&my_envp);

	return (0);
}
