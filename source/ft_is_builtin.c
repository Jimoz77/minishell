/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:01:37 by jiparcer          #+#    #+#             */
/*   Updated: 2025/03/24 16:22:47 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_echo(char *cmd, char **envp)
{
	(void)envp;
	ft_printf("%s\n", cmd);
}

void ft_pwd(void)
{
	ft_getcwd();
}





int	ft_is_builtin(char *cmd, char **envp)
{
	if (ft_strncmp(cmd, "echo", 4) == 0)
		ft_echo(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "cd", 2) == 0)
		;//ft_cmd(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd, "export", 6) == 0)
		;//ft_export(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "unset", 5) == 0)
		;//ft_unset(cmd, envp); //TODO
	else if (ft_strncmp(cmd, "env", 3) == 0)
		;//ft_env(envp); //TODO
	else if (ft_strncmp(cmd, "exit", 4) == 0)
		;//ft_exit(cmd); //TODO
	return (0);
}
