/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   envar_to_value.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 15:48:01 by jimpa             #+#    #+#             */
/*   Updated: 2025/05/12 19:14:44 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
char	*clean_double_slashes(char *path)
{
    char	*cleaned = ft_strdup(path);
    char	*dest = cleaned;
    char	*src = cleaned;
 
    while (*src)
    {
 	   *dest = *src++;
 	   if (*dest == '/' && *(src) == '/')
 		   continue; // Saute les '/' en double
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
	if(var_name[0] == '~')
		var_name = "HOME";
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
		|| c == '_' || c == '~');
}

static int	is_valid_var_char(int c)
{
	return (is_valid_var_start(c)
		|| (c >= '0' && c <= '9'));
}

static void process_parts(char ***envp, t_token *token)
{
    t_word_part *current_part;
    char *start;
    char *end;
    char *var_name;
    char *var_value;
    char *prefix;
    char *new_val;
    int i;

    current_part = token->parts;
    while (current_part != NULL)
    {
        if (current_part->type != QUOTE_SINGLE)
        {
            i = 0;
            while (current_part->content[i] != '\0' && current_part->content[i] != '$')
                i++;
            if (current_part->content[i] == '$')
            {
                prefix = ft_strndup(current_part->content, i);
                start = current_part->content + i + 1;
                if (!*start || !is_valid_var_start(*start))
                {
                    free(current_part->content);
                    current_part->content = ft_strdup("");
                    free(prefix);
                }
                else
                {
                    end = start;
                    while (*end && is_valid_var_char(*end))
                        end++;
                    var_name = ft_strndup(start, end - start);
                    var_value = get_env_value(var_name, envp);
                    new_val = ft_strjoin(prefix, var_value ? var_value : "");
                    char *temp = new_val;
                    new_val = ft_strjoin(temp, end);
                    free(temp);
                    free(current_part->content);
                    current_part->content = new_val;
                    free(var_name);
                }
                free(prefix);
            }
        }
        current_part = current_part->next;
    }
}

static void	process_variable(char ***envp, t_token *token)
{
	char	*current;
	char	*var_start;
	char	*var_end;
	char	*var_name;
	char	*var_value;
	char	*new_val;

	current = token->value;
	new_val = ft_strdup("");

	while (*current)
	{
		if (*current == '$' || *current == '~')
		{
			var_start = current + (*current == '$' ? 1 : 0);
			var_end = var_start;
			// Corriger la condition pour ignorer la vérification de '/'
			while (*var_end && is_valid_var_char(*var_end))
				var_end++;
			var_name = ft_strndup(var_start, var_end - var_start);
			var_value = get_env_value(var_name, envp);

			// Concaténer la valeur de la variable
			char *temp = ft_strjoin(new_val, var_value ? var_value : "");
			free(new_val);
			new_val = temp;

			current = var_end;
			free(var_name);
		}
		else
		{
			char str[2] = { *current, '\0' };
			char *temp = ft_strjoin(new_val, str);
			free(new_val);
			new_val = temp;
			current++;
		}
	}

	char *cleaned = clean_double_slashes(new_val);
	free(new_val);
	new_val = cleaned;

	free(token->value);
	token->value = new_val;
	process_parts(envp, token);
}

void	envar_to_value(char ***envp, t_token *token)
{
	if (!token || token->type != TOKEN_WORD)
		return ;
	if (token->value)
		process_variable(envp, token);
}

int	scan_envar(t_shell *shell)
{
	int		found;
	int 	i;
	t_token	*tmp;

	found = 0;
	tmp = shell->tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->value && tmp->value[0] == '$' && tmp->value[1] == '?')
		{
			free(tmp->value);
			tmp->value = ft_itoa(shell->exit_status); // exit_status pas toujours a jour il me semble
			found = 1;
		}
		else if (tmp->type == TOKEN_WORD && tmp->value && tmp->value[0] == '$' && tmp->value[1] == '$')
		{
			free(tmp->value);
			tmp->value = ft_itoa(getpid());
			found = 1;
		}
		else if (tmp->type == TOKEN_WORD && tmp->value)
		{
			i = 0;
			while(tmp->value[i])
			{
				if(tmp->value[i] == '$' || ((tmp->value[i] == '~' && !tmp->value[i - 1]) && (tmp->value[i + 1] == '/' || !tmp->value[i + 1])))
				{
					envar_to_value(shell->envp, tmp);
					found = 1;
				}
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (found);
}


