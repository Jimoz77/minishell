/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:01:37 by jiparcer          #+#    #+#             */
/*   Updated: 2025/05/22 15:06:34 by jiparcer         ###   ########.fr       */
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
	int j;
	int i;

	j = 0;
	i = 0;
	if (cmd[1])
	{
		while(cmd[j])
		{
			i = 0;
			while (cmd[j][i])
			{
				if(cmd[j][i] == '+' || cmd[j][i] == '-' || (cmd[j][i] >= 'a' && cmd[j][i] <= 'z') || (cmd[j][i] >= 'A' && cmd[j][i] >= 'Z'))
					exit (0);
				if(cmd[j][i] == ' ')
					i++;
				if(cmd[j][i] < 0 && cmd[j][i] > 9 && cmd[j][i] != '"')
					exit (2);
				i++;
			}
			j++;
		}
		if(cmd[1] && cmd[2])
			exit (1);
		save_env(envp);
		exit(ft_atoi(cmd[1]));
	}
	else
	{
		save_env(envp);
		exit(0);
	}
}

int	execute_builtin(char **cmd, char ***envp)
{
	if (ft_strncmp(cmd[0], "export", 6) == 0)
		return (ft_export(cmd, envp));
	else if (ft_strncmp(cmd[0], "unset", 5) == 0)
		return (ft_unset(cmd, envp));
	else if (ft_strncmp(cmd[0], "env", 3) == 0)
		return (ft_env(envp));
	else if (ft_strncmp(cmd[0], "cd", 2) == 0)
		return (ft_cd(cmd, envp));
	else if (ft_strncmp(cmd[0], "pwd", 3) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd[0], "echo", 4) == 0)
		return (ft_echo(cmd));
	else if (ft_strncmp(cmd[0], "exit", 4) == 0)
	{
		clean_exit(cmd, envp);
	}
	return (1);
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
