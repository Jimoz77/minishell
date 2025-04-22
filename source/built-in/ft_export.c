/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 14:12:26 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/03 12:53:10 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
//verifie si le nom de la variable d'environnement est valable
//car le nom d'un env_var ne peut pas commencer par un chiffre
//mais peut commencer par "_" ou une lettre

int print_sorted_env(char **envp)
{
	int		i;
	int		j;
	int		totalen;
	int		cmp_len;
	int		cmp;
	int		len1;
	int		len2;
	char	*eq;
	char	*eq1;
	char	*eq2;
	char	**envp_cpy;
	char	*temp;

	totalen = 0;
	i = 0;
	j = 0;
	while (envp[totalen])
		totalen++;
	envp_cpy = malloc(sizeof(char *) * (totalen + 1));
	if (!envp_cpy)
		return (0);
	while (i < totalen)
	{
		envp_cpy[i] = envp[i];
		i++;
	}
	envp_cpy[totalen] = NULL;
	i = 0;
	while (i < totalen - 1)
	{
		j = i + 1;
		while (j < totalen)
		{
			eq1 = ft_strchr(envp_cpy[i], '=');
			eq2 = ft_strchr(envp_cpy[j], '=');
			if (eq1)
				len1 = eq1 - envp_cpy[i];
			else
				len1 = ft_strlen(envp_cpy[i]);
			if (eq2)
				len2 = eq2 - envp_cpy[j];
			else
				len2 = ft_strlen(envp_cpy[j]);
			if (len1 < len2)
				cmp_len = len1;
			else
				cmp_len = len2;
			cmp = ft_strncmp(envp_cpy[i], envp_cpy[j], cmp_len);
			if (cmp > 0 || (cmp == 0 && len1 > len2))
			{
				temp = envp_cpy[i];
				envp_cpy[i] = envp_cpy[j];
				envp_cpy[j] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (i < totalen)
	{

		eq = ft_strchr(envp_cpy[i], '=');
		if (eq)
			printf("declare -x %.*s=\"%s\"\n", (int)(eq - envp_cpy[i]), envp_cpy[i], eq + 1); // peut etre modifier présentation suivant la machine
		else
			printf("declare -x %s\n", envp_cpy[i]);// ici aussi 
		i++;
	}
	free(envp_cpy);
	return (1);
}

char **ft_array_add(char **array, const char *str)
{
	int		count;
	char	**new;
	int		i;

	count = 0;
	i = 0;
	while (array[count])
		count++;

	new = malloc((count + 2) * sizeof(char *));
	if (!new)
		return (NULL);

	while (i < count)
	{
		new[i] = array[i];  // Recopier les anciennes valeurs
		i++;
	}
	new[count] = ft_strdup(str);  // Ajouter la nouvelle
	new[count + 1] = NULL;  // Terminer le tableau
	free(array);  // Libérer l'ancien tableau
	return (new);
}

int	is_valid_id(char *str)
{
	int	i;

	i = 1;
	if (!str || !*str || (!isalpha(*str) && *str != '_'))
		return (0);
	while (str[i])
	{
		if (!isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
// lors de l utilisation de "export test" si test a deja une valeur la fonction ne doit pas reinitialiser la valeur de test
int	ft_export(char **cmd, char ***envp)
{
	int i;
	char *egal;
	int var_len;
	char *var_name;
	char *new_entry;

	if(!cmd[1]) // export sans argument 
		return (print_sorted_env(*envp));
	egal = ft_strchr(cmd[1], '=');
	if (egal != NULL)
		var_len = (size_t)(egal - cmd[1]);
	else
		var_len = ft_strlen(cmd[1]);
	var_name = ft_substr(cmd[1], 0, var_len);
	if(!is_valid_id(var_name))
	{
		ft_putstr_fd("export: not a valid identifier\n", STDERR_FILENO);
		free(var_name);
		return (0);
	}
	i = 0;
	while((*envp)[i])
	{
		if(ft_strncmp((*envp)[i], var_name, var_len) == 0 && ft_strlen((*envp)[i]) > (size_t)var_len && (*envp)[i][var_len] == '=')
		{
			if (egal) // Seulement si on a un '=' dans la commande
			{
				free((*envp)[i]);
				(*envp)[i] = ft_strdup(cmd[1]);
				free(var_name);
				return (1);
			}
			else // Si la variable existe déjà et pas de '=' : ne rien faire
			{
				free(var_name);
				return (1);
			}
		}
		i++;
	}
	if (egal != NULL)
		new_entry = ft_strdup(cmd[1]);
	else
		new_entry = ft_strjoin(var_name, "=");
	*envp = ft_array_add(*envp, new_entry);
	free(new_entry);
	free(var_name);
	return (1);
}
