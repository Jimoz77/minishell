/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:10:23 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:31:59 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	append_char(char **str, char c)
{
	char	buf[2];
	char	*temp;

	buf[0] = c;
	buf[1] = '\0';
	temp = ft_strjoin(*str, buf);
	free(*str);
	*str = temp;
}

void	finalize_content(t_word_part *part, char *new_content)
{
	char	*cleaned;

	cleaned = clean_double_slashes(new_content);
	free(part->content);
	part->content = cleaned;
	free(new_content);
}

void	append_str(char **str, const char *append)
{
	char	*tmp;

	tmp = ft_strjoin(*str, append);
	free(*str);
	*str = tmp;
}

int	is_valid_var_start(int c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_' || c == '~');
}

int	is_valid_var_char(int c)
{
	return (is_valid_var_start(c)
		|| (c >= '0' && c <= '9'));
}
