/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:01:37 by jiparcer          #+#    #+#             */
/*   Updated: 2025/07/19 16:12:29 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void free_exit(int cmd, t_shell *shell)
{
	if (shell)
		free_shell(shell);
	exit(cmd);
}

static int	is_invalid_exit_arg(char **cmd)
{
	int	j;
	int	i;

	j = 1;
	while (cmd[j])
	{
		i = 0;
		while (cmd[j][i])
		{
			if (cmd[j][i] == ' ')
				i++;
			if (cmd[j][i] == '+' || cmd[j][i] == '-'
				|| ft_isalpha(cmd[j][i]))
				return (1);
			i++;
		}
		j++;
	}
	return (0);
}

void	clean_exit(char **cmd, char ***envp, t_shell *shell)
{
	if (!cmd[1])
	{
		save_env(*envp);
		free_exit(0, shell);
	}
	if (cmd[1] && !cmd[2])
	{
		if (cmd[1][0] == '+' || cmd[1][0] == '-')
		{
			if (cmd[1][1] == '0' && cmd[1][2] == '\0')
				free_exit(0, shell);
		}
		if (is_invalid_exit_arg(cmd))
			free_exit(2, shell);
		save_env(*envp);
		free_exit(ft_atoi(cmd[1]), shell);
	}
	if (cmd[1] && cmd[2])
	{
		if (ft_isalpha(cmd[1][0]))
			free_exit(2, shell);
		free_exit(1, shell);
	}
}

int	execute_builtin(char **cmd, char ***envp, t_shell *shell)
{
	if (ft_strcmp(cmd[0], "export") == 0)
		return (ft_export(cmd, envp));
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (ft_unset(cmd, envp));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (ft_env(envp));
	else if (ft_strcmp(cmd[0], "cd") == 0)
		return (ft_cd(cmd, envp));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (ft_echo(cmd));
	else if (ft_strcmp(cmd[0], "exit") == 0)
	{
		clean_exit(cmd, envp, shell);
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
