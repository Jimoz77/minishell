/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:08:22 by jimpa             #+#    #+#             */
/*   Updated: 2025/03/31 19:55:33 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_cd(char **cmd)
{
	char		cwd[BUFSIZ];
	char		*new_dir;
	static char	*old_pwd;
	char		*tmp_pwd;

	if (!old_pwd)
		old_pwd = getenv("OLDPWD");
	if (cmd[1] == NULL || (cmd[1][0] == '-' && cmd[1][1] == '-')) //peut etre mauvaise gestion de old_pwd pour utiliser "cd -"
		chdir(getenv("HOME"));
	else if(cmd[1][0] == '-' && cmd[1][1] == '\0')
	{
		getcwd(cwd,sizeof(cwd));
		tmp_pwd = ft_strdup(cwd);
		chdir(old_pwd);
		old_pwd = tmp_pwd;
	}
	else
	{
		getcwd(cwd, sizeof(cwd));
		old_pwd = ft_strdup(cwd);
		new_dir = ft_strjoin(cwd, "/");
		new_dir = ft_strjoin(new_dir, cmd[1]);
		if ((chdir(new_dir)) != 0)
		{
			printf("bash: cd: %s: Aucun fichier ou dossier de ce nom\n", cmd[1]);
			old_pwd = NULL;
		}
	}
	return (1);
}
