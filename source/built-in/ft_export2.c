/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 19:04:55 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 19:13:01 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
