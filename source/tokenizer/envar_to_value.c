/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:48:01 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/17 19:40:07 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

size_t	ft_strnlen(const char *s, size_t maxlen)
{
	size_t len;

	len = 0;
	while (len < maxlen && s[len])
		len++;
	return (len);
}
// ft_strndup.c
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

// ft_strnlen.c


static char	*get_env_value(const char *var_name, char ***envp)
{
	int		i;
	size_t	name_len;
	size_t	env_len;
	char	*eq_pos;

	if (!envp || !*envp)
		return (NULL);
	name_len = ft_strlen(var_name);
	i = 0;
	while ((*envp)[i])
	{
		eq_pos = ft_strchr((*envp)[i], '=');
		if (eq_pos)
		{
			env_len = eq_pos - (*envp)[i];
			if (env_len == name_len
				&& !ft_strncmp((*envp)[i], var_name, name_len))
				return (eq_pos + 1);
		}
		i++;
	}
	return (NULL);
}

static int	is_valid_var_start(int c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_');
}

static int	is_valid_var_char(int c)
{
	return (is_valid_var_start(c)
		|| (c >= '0' && c <= '9'));
}

static void	process_variable(char ***envp, t_token *token)
{
	char	*start;
	char	*end;
	char	*var_name;
	char	*var_value;
	char	*new_val;

	start = token->value + 1;
	if (!*start || !is_valid_var_start(*start))
	{
		free(token->value);
		token->value = ft_strdup("");
		return ;
	}
	end = start;
	while (*end && is_valid_var_char(*end))
		end++;
	var_name = ft_strndup(start, end - start);
	var_value = get_env_value(var_name, envp);
	new_val = ft_strjoin(var_value ? var_value : "", end);
	free(token->value);
	free(var_name);
	token->value = new_val;
}

void	envar_to_value(char ***envp, t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
		return ;
	if (token->value && token->value[0] == '$')
		process_variable(envp, token);
}

int	scan_envar(t_token *tokens, char ***envp)
{
	int		found;
	t_token	*tmp;

	found = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->value && tmp->value[0] == '$')
		{
			envar_to_value(envp, tmp);
			found = 1;
		}
		tmp = tmp->next;
	}
	return (found);
}

