/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value8.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:15:02 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/20 03:30:47 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_character_loop(t_shell *shell, t_token *token,
								char **new_val)
{
	char	*current;

	current = token->value;
	while (*current)
	{
		if (should_process_variable(current, token))
		{
			if (handle_numeric_parameter(&current))
				continue ;
			process_single_variable(&current, new_val, shell);
		}
		else
		{
			append_single_char(new_val, *current);
			current++;
		}
	}
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
