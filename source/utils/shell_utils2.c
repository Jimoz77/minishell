/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:02:24 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/10 19:20:38 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Libère proprement la structure principale du shell
void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->envp)
	{
		i = 0;
		while (shell->envp[i])
			free_array(shell->envp[i++]);
		free(shell->envp);
	}
	if (shell->tokens)
		free_tokens(shell->tokens);
	if (shell->ast)
		free_ast(shell->ast);
	if (shell->current_dir)
		free(shell->current_dir);
	if (shell->old_cmd)
		free(shell->old_cmd);
	shell->old_cmd = NULL;
	free(shell);
}

char	*trim_left(char *str)
{
	if (!str)
		return (NULL);
	while (*str && (*str == ' ' || *str == '\t'
			|| *str == '\n' || *str == '\r'))
		str++;
	while (*str && (unsigned char)*str == 194
		&& str[1] && (unsigned char)str[1] == 160)
		str += 2;
	while (*str && (*str == ' ' || *str == '\t'
			|| *str == '\n' || *str == '\r'))
		str++;
	return (str);
}

char	**create_simple_env(void)
{
	char	**envp;

	envp = malloc(sizeof(char *) * 4);

	envp[0] = malloc(sizeof("PWD=/home/lsadikaj/Documents/minishell"));
	envp[0] = ft_strdup("PWD=/home/lsadikaj/Documents/minishell");
	envp[1] = malloc(sizeof("SHLVL=1"));
	envp[1] = ft_strdup("SHLVL=1");
	envp[2] = malloc(sizeof("_=/usr/bin/env"));
	envp[2] = ft_strdup("_=/usr/bin/env");
	envp[3] = '\0';
	return (envp);
}

char	***decrement_shlvl(char ***envp)
{
	int i;
	int	j;
	int lvl;

	i = 0;
	j = 0;
	while(envp[0][i] && ft_strncmp(envp[0][i], "SHLVL", 5))
		i++;
	while (envp[0][i][j] && envp[0][i][j] != '=')
		j++;
	j++;
	lvl = envp[0][i][j] - '0';
	lvl--;
	envp[0][i][j] = lvl + '0';
	return (envp);
}

char	***increment_shlvl(char ***envp)
{
	int i;
	int	j;
	int lvl;

	i = 0;
	j = 0;
	while(envp[0][i] && ft_strncmp(envp[0][i], "SHLVL", 5))
		i++;
	while (envp[0][i][j] && envp[0][i][j] != '=')
		j++;
	j++;
	lvl = envp[0][i][j] - '0';
	lvl++;
	envp[0][i][j] = lvl + '0';
	return (envp);
}
