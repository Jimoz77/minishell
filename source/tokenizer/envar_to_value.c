/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:48:01 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/17 15:52:34 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t	len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}

char	*ft_strndup(char *s, size_t n)
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

char	*get_env_value_str(const char *var_name, char ***envp)
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
