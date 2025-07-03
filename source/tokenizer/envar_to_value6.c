/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value6.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:13:25 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:43:43 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_dollar_variable(char *current)
{
	return (*current == '$');
}

int	is_tilde_variable(char *current, t_token *token)
{
	return (*current == '~' && !token->parts);
}

int	should_process_variable(char *current, t_token *token)
{
	if (is_dollar_variable(current))
		return (1);
	if (is_tilde_variable(current, token))
		return (1);
	return (0);
}

int	handle_numeric_parameter(char **current)
{
	if ((*current)[0] == '$' && ((*current)[1] >= '0' && (*current)[1] <= '9'))
	{
		*current += 2;
		return (1);
	}
	return (0);
}

char	*get_var_start_position(char *current)
{
	if (*current == '$')
		return (current + 1);
	return (current);
}
