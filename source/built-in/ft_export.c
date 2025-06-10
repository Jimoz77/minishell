/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:12:26 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 19:25:14 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//verifie si le nom de la variable d'environnement est valable
//car le nom d'un env_var ne peut pas commencer par un chiffre
//mais peut commencer par "_" ou une lettre
static char	*get_var_name(const char *arg)
{
	char	*egal;
	int		len;

	egal = ft_strchr(arg, '=');
	if (egal)
		len = egal - arg;
	else
		len = ft_strlen(arg);
	return (ft_substr(arg, 0, len));
}

static int	update_existing_var(char *arg, char *var_name, char ***envp)
{
	int		i;
	int		len;
	char	*egal;

	egal = ft_strchr(arg, '=');
	len = ft_strlen(var_name);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var_name, len) == 0
			&& (*envp)[i][len] == '=')
		{
			if (egal)
			{
				free((*envp)[i]);
				(*envp)[i] = ft_strdup(arg);
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static void	add_new_var(char *arg, char *var_name, char ***envp)
{
	char	*entry;

	if (ft_strchr(arg, '='))
		entry = ft_strdup(arg);
	else
		entry = ft_strjoin(var_name, "=");
	*envp = ft_array_add(*envp, entry);
	free(entry);
}

int	ft_export(char **cmd, char ***envp)
{
	char	*var_name;
	int		ret;

	if (!cmd[1])
		return (print_sorted_env(*envp));
	var_name = get_var_name(cmd[1]);
	if (!is_valid_id(var_name))
	{
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
		if (cmd[1][0] == '-')
			ret = 2;
		else
			ret = 1;
		free(var_name);
		return (ret);
	}
	if (!update_existing_var(cmd[1], var_name, envp))
		add_new_var(cmd[1], var_name, envp);
	if (var_name)
		free(var_name);
	return (0);
}
