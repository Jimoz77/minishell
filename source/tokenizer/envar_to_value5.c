/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:12:31 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:43:02 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_var_value_safe(char *var_name, char ***envp)
{
	char	*var_value;

	var_value = get_env_value(var_name, envp);
	if (var_value)
		return (var_value);
	return ("");
}

static void	replace_var_content(char **new_content, char *var_name,
							char ***envp)
{
	char	*var_value;
	char	*temp;

	var_value = get_var_value_safe(var_name, envp);
	temp = ft_strjoin(*new_content, var_value);
	free(*new_content);
	*new_content = temp;
}

static void	process_variable_expansion(char **current, char **new_content,
									char *var_start, t_shell *shell)
{
	char	*var_end;
	char	*var_name;

	var_end = find_var_end(var_start);
	var_name = ft_strndup(var_start, var_end - var_start);
	replace_var_content(new_content, var_name, shell->envp);
	*current = var_end;
	free(var_name);
}

static void	process_dollar(char **current, char **new_content, t_shell *shell)
{
	char	*var_start;

	(*current)++;
	var_start = *current;
	if (handle_exit_status(current, shell))
		return ;
	if (handle_shell_pid(current))
		return ;
	if (handle_numeric_param(current))
		return ;
	if (handle_invalid_var(var_start, new_content))
		return ;
	process_variable_expansion(current, new_content, var_start, shell);
}

void	process_parts(t_shell *shell, t_token *token)
{
	t_process_parts	v;

	v.current_part = token->parts;
	while (v.current_part)
	{
		if (v.current_part->type != QUOTE_SINGLE)
		{
			v.current = v.current_part->content;
			v.new_content = ft_strdup("");
			while (*v.current)
			{
				if (*v.current == '$')
				{
					process_dollar(&v.current, &v.new_content, shell);
				}
				else
				{
					append_char(&v.new_content, *v.current);
					v.current++;
				}
			}
			finalize_content(v.current_part, v.new_content);
		}
		v.current_part = v.current_part->next;
	}
}
