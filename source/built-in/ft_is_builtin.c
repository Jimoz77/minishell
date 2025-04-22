/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:01:37 by jiparcer          #+#    #+#             */
/*   Updated: 2025/04/21 15:44:49 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
// a utiliser dans une condition if 
//en verifiant aussi que le type de la valeur utilisé soit "WORD"
//si la commande entré est un built-in alors
//la fonction retourne 1 et 0 si ca ne l'est pas
// (manque encore "exit" a coder)

void	clean_exit(char **cmd, char ***envp)
{
	if (cmd[1])
	{
		save_env(envp);
		write_history("etc/.minishell_history");
		exit(ft_atoi(cmd[1]));
	}
	else
	{
		save_env(envp);
		write_history("etc/.minishell_history");
		exit(0);
	}
}

void	execute_builtin(char **cmd, char ***envp)
{
	if (ft_strncmp(cmd[0], "export", 6) == 0)
		ft_export(cmd, envp);
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		ft_unset(cmd, envp);
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		ft_env(envp);
	else if (ft_strncmp(cmd[0], "cd", 2) == 0)
		ft_cd(cmd, envp);
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		ft_pwd();
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		ft_echo(cmd);
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
	{
		clean_exit(cmd, envp);
	}
}

// Modifier ft_is_builtin pour retourner 1 si c'est un builtin
int	ft_is_builtin(char **cmd, char ***envp)
{
	(void)envp;
	if (!cmd || !cmd[0])
		return (0);
	return (ft_strncmp(cmd[0], "echo", 4) == 0
		|| ft_strncmp(cmd[0], "cd", 2) == 0
		|| ft_strncmp(cmd[0], "pwd", 3) == 0
		|| ft_strncmp(cmd[0], "export", 6) == 0
		|| ft_strncmp(cmd[0], "unset", 5) == 0
		|| ft_strncmp(cmd[0], "env", 3) == 0
		|| ft_strncmp(cmd[0], "exit", 4) == 0);
}









/* int	ft_is_builtin(char **cmd, char ***envp)
{
	if (ft_strncmp(cmd[0], "echo", 4) == 0)
		return (ft_echo(cmd));
	else if (ft_strncmp(cmd[0], "cd", 2) == 0)
		return (ft_cd(cmd, envp));
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd[0], "export", 6) == 0)
		return (ft_export(cmd, envp));
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		return (ft_unset(cmd, envp));
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		return (ft_env(envp));
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
		return (1);									//ft_exit(cmd, envp) //TODO
	else
		return (0); // ce n'est pas un builtin
}
 */