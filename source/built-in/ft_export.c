/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:12:26 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 18:10:06 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//verifie si le nom de la variable d'environnement est valable
//car le nom d'un env_var ne peut pas commencer par un chiffre
//mais peut commencer par "_" ou une lettre

static int	get_envp_len(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

static int	copy_envp_to_struct(t_printsort *v, char **envp)
{
	int	i;

	v->totalen = get_envp_len(envp);
	v->envp_cpy = malloc(sizeof(char *) * (v->totalen + 1));
	if (!v->envp_cpy)
		return (0);
	i = 0;
	while (i < v->totalen)
	{
		v->envp_cpy[i] = envp[i];
		i++;
	}
	v->envp_cpy[i] = NULL;
	return (1);
}

static void	swap_if_needed(t_printsort *v)
{
	v->eq1 = ft_strchr(v->envp_cpy[v->i], '=');
	v->eq2 = ft_strchr(v->envp_cpy[v->j], '=');
	if (v->eq1)
		v->len1 = v->eq1 - v->envp_cpy[v->i];
	else
		v->len1 = ft_strlen(v->envp_cpy[v->i]);
	if (v->eq2)
		v->len2 = v->eq2 - v->envp_cpy[v->j];
	else
		v->len2 = ft_strlen(v->envp_cpy[v->j]);
	if (v->len1 < v->len2)
		v->cmp_len = v->len1;
	else
		v->cmp_len = v->len2;
	v->cmp = ft_strncmp(v->envp_cpy[v->i], v->envp_cpy[v->j], v->cmp_len);
	if (v->cmp > 0 || (v->cmp == 0 && v->len1 > v->len2))
	{
		v->temp = v->envp_cpy[v->i];
		v->envp_cpy[v->i] = v->envp_cpy[v->j];
		v->envp_cpy[v->j] = v->temp;
	}
}

static void	sort_envp_cpy(t_printsort *v)
{
	v->i = 0;
	while (v->i < v->totalen - 1)
	{
		v->j = v->i + 1;
		while (v->j < v->totalen)
		{
			swap_if_needed(v);
			v->j++;
		}
		v->i++;
	}
}

static void	print_envp_lines(t_printsort *v)
{
	v->i = 0;
	while (v->i < v->totalen)
	{
		v->eq = ft_strchr(v->envp_cpy[v->i], '=');
		if (v->eq)
		{
			printf("declare -x %.*s=\"%s\"\n",
				(int)(v->eq - v->envp_cpy[v->i]),
				v->envp_cpy[v->i], v->eq + 1);
		}
		else
			printf("declare -x %s\n", v->envp_cpy[v->i]);
		v->i++;
	}
}

int	print_sorted_env(char **envp)
{
	t_printsort	v;

	if (!copy_envp_to_struct(&v, envp))
		return (0);
	sort_envp_cpy(&v);
	print_envp_lines(&v);
	free(v.envp_cpy);
	return (1);
}

char	**ft_array_add(char **array, const char *str)
{
	int		count;
	char	**new;
	int		i;

	count = 0;
	i = 0;
	while (array[count])
		count++;
	new = malloc((count + 2) * sizeof(char *));
	if (!new)
		return (NULL);
	while (i < count)
	{
		new[i] = array[i];
		i++;
	}
	new[count] = ft_strdup(str);
	new[count + 1] = NULL;
	free(array);
	return (new);
}

int	is_valid_id(char *str)
{
	int	i;

	i = 1;
	if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

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

