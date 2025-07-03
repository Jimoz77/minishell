/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value7.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:14:15 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:45:03 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*find_variable_end(char *var_start)
{
	char	*var_end;

	var_end = var_start;
	while (*var_end && is_valid_var_char(*var_end))
		var_end++;
	return (var_end);
}

static char	*get_variable_value_safe(char *var_name, char ***envp)
{
	char	*var_value;

	var_value = get_env_value(var_name, envp);
	if (var_value)
		return (var_value);
	return ("");
}

static void	append_variable_value(char **new_val, char *var_name, char ***envp)
{
	char	*var_value;
	char	*temp;

	var_value = get_variable_value_safe(var_name, envp);
	temp = ft_strjoin(*new_val, var_value);
	free(*new_val);
	*new_val = temp;
}

void	process_single_variable(char **current, char **new_val,
								t_shell *shell)
{
	char	*var_start;
	char	*var_end;
	char	*var_name;

	var_start = get_var_start_position(*current);
	var_end = find_variable_end(var_start);
	var_name = ft_strndup(var_start, var_end - var_start);
	append_variable_value(new_val, var_name, shell->envp);
	*current = var_end;
	free(var_name);
}

void	append_single_char(char **new_val, char c)
{
	char	str[2];
	char	*temp;

	str[0] = c;
	str[1] = '\0';
	temp = ft_strjoin(*new_val, str);
	free(*new_val);
	*new_val = temp;
}
