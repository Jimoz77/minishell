/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:48:01 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 01:36:58 by jimpa            ###   ########.fr       */
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

static char	*get_env_value(const char *var_name, char ***envp)
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

static void	append_char(char **str, char c)
{
	char	buf[2];
	char	*temp;

	buf[0] = c;
	buf[1] = '\0';
	temp = ft_strjoin(*str, buf);
	free(*str);
	*str = temp;
}

static void	finalize_content(t_word_part *part, char *new_content)
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

static int	is_valid_var_start(int c)
{
	return ((c >= 'a' && c <= 'z')
		|| (c >= 'A' && c <= 'Z')
		|| c == '_' || c == '~');
}

static int	is_valid_var_char(int c)
{
	return (is_valid_var_start(c)
		|| (c >= '0' && c <= '9'));
}

static int	handle_exit_status(char **current, t_shell *shell)
{
	if ((*current)[0] == '?')
	{
		free(*current);
		*current = ft_itoa(shell->exit_status);
		return (1);
	}
	return (0);
}

static int	handle_shell_pid(char **current)
{
	if ((*current)[0] == '$')
	{
		free(*current);
		*current = ft_itoa(get_shell_pid());
		return (1);
	}
	return (0);
}

static int	handle_numeric_param(char **current)
{
	if ((*current)[0] >= '0' && (*current)[0] <= '9')
	{
		(*current)++;
		return (1);
	}
	return (0);
}

static int	handle_invalid_var(char *var_start, char **new_content)
{
	if (!var_start || !is_valid_var_start(*var_start))
	{
		append_char(new_content, '$');
		return (1);
	}
	return (0);
}

static char	*find_var_end(char *var_start)
{
	char	*var_end;

	var_end = var_start;
	while (*var_end && is_valid_var_char(*var_end))
		var_end++;
	return (var_end);
}

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

static void	process_parts(t_shell *shell, t_token *token)
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

static int	is_dollar_variable(char *current)
{
	return (*current == '$');
}

static int	is_tilde_variable(char *current, t_token *token)
{
	return (*current == '~' && !token->parts);
}

static int	should_process_variable(char *current, t_token *token)
{
	if (is_dollar_variable(current))
		return (1);
	if (is_tilde_variable(current, token))
		return (1);
	return (0);
}

static int	handle_numeric_parameter(char **current)
{
	if ((*current)[0] == '$' && ((*current)[1] >= '0' && (*current)[1] <= '9'))
	{
		*current += 2;
		return (1);
	}
	return (0);
}

static char	*get_var_start_position(char *current)
{
	if (*current == '$')
		return (current + 1);
	return (current);
}

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

static void	process_single_variable(char **current, char **new_val,
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

static void	append_single_char(char **new_val, char c)
{
	char	str[2];
	char	*temp;

	str[0] = c;
	str[1] = '\0';
	temp = ft_strjoin(*new_val, str);
	free(*new_val);
	*new_val = temp;
}

static void	process_character_loop(t_shell *shell, t_token *token,
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

static void	finalize_token_value(t_token *token, char *new_val)
{
	char	*cleaned;

	cleaned = clean_double_slashes(new_val);
	free(new_val);
	free(token->value);
	token->value = cleaned;
}

static void	process_variable(t_shell *shell, t_token *token)
{
	char	*new_val;

	new_val = ft_strdup("");
	process_character_loop(shell, token, &new_val);
	finalize_token_value(token, new_val);
	process_parts(shell, token);
}

void	envar_to_value(t_shell *shell, t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
		return ;
	if (token->value)
		process_variable(shell, token);
}

int	scan_envar_parsing_phase(t_shell *shell)
{
	int		found;
	t_token	*tmp;

	found = 0;
	tmp = shell->tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->value)
		{
			if (tmp->value[0] == '$' && tmp->value[1] == '$')
			{
				free(tmp->value);
				tmp->value = ft_itoa(get_shell_pid());
				found = 1;
			}
		}
		else if (is_redirection(tmp->type)
			&& tmp->next && tmp->next->type == TOKEN_WORD)
		{
			envar_to_value(shell, tmp->next);
			found = 1;
		}
		tmp = tmp->next;
	}
	return (found);
}

// Fonction d'expansion complète à l'exécution
int	scan_envar_execution_phase(t_shell *shell, t_token *tokens)
{
	int		found;
	t_token	*tmp;

	found = 0;
	tmp = tokens;
	if (tmp->type == TOKEN_WORD && tmp->value)
	{
		if (tmp->value[0] == '$' && tmp->value[1] == '?')
		{
			free(tmp->value);
			tmp->value = ft_itoa(shell->exit_status);
			found = 1;
		}
		else
		{
			envar_to_value(shell, tmp);
		}
	}
	return (found);
}
