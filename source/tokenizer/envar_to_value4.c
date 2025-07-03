/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:11:44 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:33:54 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_exit_status(char **current, t_shell *shell)
{
	if ((*current)[0] == '?')
	{
		free(*current);
		*current = ft_itoa(shell->exit_status);
		return (1);
	}
	return (0);
}

int	handle_shell_pid(char **current)
{
	if ((*current)[0] == '$')
	{
		free(*current);
		*current = ft_itoa(get_shell_pid());
		return (1);
	}
	return (0);
}

int	handle_numeric_param(char **current)
{
	if ((*current)[0] >= '0' && (*current)[0] <= '9')
	{
		(*current)++;
		return (1);
	}
	return (0);
}

int	handle_invalid_var(char *var_start, char **new_content)
{
	if (!var_start || !is_valid_var_start(*var_start))
	{
		append_char(new_content, '$');
		return (1);
	}
	return (0);
}

char	*find_var_end(char *var_start)
{
	char	*var_end;

	var_end = var_start;
	while (*var_end && is_valid_var_char(*var_end))
		var_end++;
	return (var_end);
}
