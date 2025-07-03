/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 15:02:24 by jimpa             #+#    #+#             */
/*   Updated: 2025/07/03 15:04:27 by jimpa            ###   ########.fr       */
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
