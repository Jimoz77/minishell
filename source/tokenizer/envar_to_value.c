/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:48:01 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/10 13:50:08 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*clean_dspace(char *str)
{
	int		i;
	int		j;
	char	tmp[BUFSIZ];
	char	*result;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] == ' ' && str[i + 1] == ' ')
			i++;
		tmp[j] = str[i];
		i++;
		j++;
	}
	tmp[j] = '\0';
	result = ft_strdup(tmp);
	return (result);
}

char	*clean_double_slashes(char *path)
{
	char	*cleaned;
	char	*dest;
	char	*src;

	cleaned = ft_strdup(path);
	dest = cleaned;
	src = cleaned;
	while (*src)
	{
		*dest = *src++;
		if (*dest == '/' && *(src) == '/')
			continue ;
		dest++;
	}
	*dest = '\0';
	return (cleaned);
}

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*new;
	size_t	len;

	len = ft_strnlen(s, n);
	new = (char *)malloc(len + 1);
	if (!new)
		return (NULL);
	ft_memcpy(new, s, len);
	new[len] = '\0';
	return (new);
}

char	*get_env_value(const char *var_name, char ***envp)
{
	t_get_env_value	v;

	if (!envp || !*envp)
		return (NULL);
	if (var_name[0] == '~')
		var_name = "HOME";
	v.i = 0;
	while ((*envp)[v.i])
	{
		v.name_len = ft_strlen(var_name);
		while (v.name_len > 0)
		{
			v.eq_pos = ft_strchr((*envp)[v.i], '=');
			if (v.eq_pos)
			{
				v.env_len = v.eq_pos - (*envp)[v.i];
				if (v.env_len == v.name_len
					&& !ft_strncmp((*envp)[v.i], var_name, v.name_len))
					return (v.eq_pos + 1);
			}
			v.name_len--;
		}
		v.i++;
	}
	return (NULL);
}

char *get_env_value_str(const char *var_name, char ***envp)
{
	t_get_env_value	v;

	if (!envp || !*envp)
		return (NULL);
	if (var_name[0] == '~')
		var_name = "HOME";
	v.i = 0;
	while ((*envp)[v.i])
	{
		v.name_len = ft_strlen(var_name);
		v.eq_pos = ft_strchr((*envp)[v.i], '=');
		if (v.eq_pos)
		{
			v.env_len = v.eq_pos - (*envp)[v.i];
			if (v.env_len == v.name_len
				&& !ft_strncmp((*envp)[v.i], var_name, v.name_len))
				return (v.eq_pos + 1);
		}
		v.i++;
	}
	return (NULL);
}
