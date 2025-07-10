/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc_expansion.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 15:37:37 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/07/10 19:03:52 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	can_expand_heredoc(char *var_end)
{
	if (!*var_end)
		return (1);
	if ((*var_end >= 'a' && *var_end <= 'z') || (*var_end >= 'A' && *var_end <= 'Z')
		|| (*var_end >= '0' && *var_end <= '9') || *var_end == '_')
		return (0);
	return (1);
}

static char	*expand_exit_status(char *line, char *current, t_shell *shell)
{
	char	*var_value;
	char	*before;
	char	*after;
	char	*temp;
	char	*result;

	var_value = ft_itoa(shell->exit_status);
	before = ft_strndup(line, current - line);
	after = ft_strdup(current + 2);
	temp = ft_strjoin(before, var_value);
	result = ft_strjoin(temp, after);
	free(before);
	free(after);
	free(temp);
	free(var_value);
	free(line);
	return (result);
}

static char	*expand_env_var(char *line, char *current, char *var_end,
							t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	char	*before;
	char	*after;
	char	*temp;

	var_name = ft_strndup(current + 1, var_end - current - 1);
	var_value = get_env_value_str(var_name, shell->envp);
	if (!var_value)
		var_value = "";
	before = ft_strndup(line, current - line);
	after = ft_strdup(var_end);
	temp = ft_strjoin(before, var_value);
	free(before);
	before = ft_strjoin(temp, after);
	free(temp);
	free(after);
	free(var_name);
	free(line);
	
	return (before);
}

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*current;
	char	*var_start;
	char	*var_end;

	if (!line || !shell)
		return (line);
	current = line;
	while (*current)
	{
		if (*current == '$')
		{
			var_start = current + 1;
			if (*var_start == '?')
				return (expand_exit_status(line, current, shell));
			var_end = find_var_end(var_start);
			if (var_end > var_start && can_expand_heredoc(var_end))
				return (expand_env_var(line, current, var_end, shell));
		}
		current++;
	}
	return (line);
}
