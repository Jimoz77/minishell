/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:01:37 by jiparcer          #+#    #+#             */
/*   Updated: 2025/03/24 17:36:49 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int ft_pwd(void)
{
	char cwd[BUFSIZ];

	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	return (1);
}


int	ft_is_builtin(char *cmd, char **envp)
{
	(void)envp; // sera utile plus tard pour les fonctions built-in
	if (ft_strncmp(cmd, "echo", 4) == 0)
		return (1);									//ft_echo(cmd); //TODO
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		return (1);									//ft_cd(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		return(ft_pwd());
	else if (ft_strncmp(cmd, "export", 6) == 0)
		return (1);									//ft_export(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		return(1);									//ft_unset(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "env", 3) == 0)
		return(1);									//ft_env(envp); //TODO
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		return (1);									//ft_exit(cmd, envp) //TODO
	else
		return (0); // ce n'est pas un builtin
}
