/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:08:22 by jimpa             #+#    #+#             */
/*   Updated: 2025/06/10 04:03:54 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//sert a update toutes les env_var
//verifier si OLDPWD est recrée apres une utilisation de CD + suppresion de OLDPWD au préalable
// si oui ajouter fonctionnalité pour rajouter une env_var si elle manque
//demander a gpt quel sont les buit-in qui modifient des env_var

char	*ft_getenv(char **envp, const char *var)
{
	int	var_len;
	int	i;

	var_len = ft_strlen(var);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
			return (&envp[i][var_len + 1]);
		i++;
	}
	return (NULL);
}

void	ft_setenv(char ***envp, char *var, char *new_val)
{
	char	*part1;
	char	*new_entry;
	int		var_len;
	int		i;

	part1 = ft_strjoin(var, "=");
	new_entry = ft_strjoin(part1, new_val);
	free(part1);
	var_len = ft_strlen(var);
	i = 0;
	while ((*envp)[i] != NULL)
	{
		if (ft_strncmp((*envp)[i], var, var_len) == 0 && \
		(*envp)[i][var_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_entry;
			return ;
		}
		i++;
	}
	free(new_entry);
}

int	ft_cd2(char **cmd, char ***envp, char *old_pwd, int result)
{
	char	*tmp_pwd;
	char	cwd[BUFSIZ];

	if (cmd[1] && cmd[1][0] == '-' && cmd[1][1] == '\0')
	{
		getcwd(cwd, sizeof(cwd));
		tmp_pwd = ft_strdup(cwd);
		old_pwd = ft_getenv(*envp, "OLDPWD");
		chdir(old_pwd);
		old_pwd = tmp_pwd;
		result = 0; 
	}
	else
	{
		getcwd(cwd, sizeof(cwd));
		old_pwd = ft_strdup(cwd);
		if (cmd[1] && (chdir(cmd[1])) == -1)
		{
			//printf("bash: cd: %s: Aucun fichier ou dossier de ce nom\n", cmd[1]); /// !!!! ATTENTION A REMPLACER PAR PERROR OU EQUIVALENT POUR PRINT EN STDERR
			free(old_pwd);
			old_pwd = NULL;
			result = 1;
		}
	}
	ft_setenv(envp, "OLDPWD", old_pwd);
	ft_setenv(envp, "PWD", getcwd(cwd, sizeof(cwd)));
	if (old_pwd)
		free(old_pwd);
	return (result);
}

int	ft_cd(char **cmd, char ***envp)
{
	char		cwd[BUFSIZ];
	static char		*old_pwd;
	int			result;

	result = 0;
	//old_pwd = getenv("OLDPWD");
	if (cmd[1] == NULL || (cmd[1][0] == '-' && cmd[1][1] == '-'))
	{
		getcwd(cwd, sizeof(cwd));
		old_pwd = ft_strdup(cwd);
		chdir(ft_getenv(*envp, "HOME"));
		ft_setenv(envp, "OLDPWD", old_pwd);
		ft_setenv(envp, "PWD", getcwd(cwd, sizeof(cwd)));
		return (0);
	}
	return (ft_cd2(cmd, envp, old_pwd, result));
}
