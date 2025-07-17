/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 13:59:25 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/17 17:18:01 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char **av, char **envp)
{
	char	**my_envp;

	(void) ac;
	(void) av;
	setup_signals();
	if (envp && envp[0])
	{
		save_env(envp);
		my_envp = load_env();
	}
	else
		my_envp = create_simple_env();
	ft_read_line(my_envp);
	if (my_envp)
		free_array(my_envp);
	return (0);
}
