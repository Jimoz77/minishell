/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcards_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jiparcer <jiparcer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:20:53 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/09 20:56:15 by jiparcer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Met à jour la liste de tokens en insérant un nouveau token.
// Si prev est NULL, cela signifie qu'on remplace le premier token de la liste.
void	update_tokens_list(t_token **tokens, t_token *prev, t_token *new_token)
{
	if (!prev)
		*tokens = new_token;
	else
		prev->next = new_token;
}

// Vérifie si un fichier est caché (commence par '.') et si le motif
// ne commence pas par '.' également. Retourne 1 si c'est un fichier caché
// à ignorer, 0 sinon.
int	is_hidden_file(const char *filename, const char *pattern)
{
	return (filename[0] == '.' && pattern[0] != '.');
}

// Détermine si un fichier doit être ignoré lors de l'expansion des wildcards.
// Pour l'instant, vérifie uniquement si c'est un fichier caché.
int	should_skip_file(const char *filename, const char *pattern)
{
	if (is_hidden_file(filename, pattern))
		return (1);
	return (0);
}

// Combine un répertoire et un nom de fichier pour former un chemin complet.
// Gère le cas où le répertoire ne se termine pas par '/'.
char	*join_path(const char *dir, const char *file)
{
	char	*path;
	int		dir_len;
	int		file_len;

	dir_len = ft_strlen(dir);
	file_len = ft_strlen(file);
	path = malloc(dir_len + file_len + 2);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, dir_len + 1);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
	{
		path[dir_len] = '/';
		ft_strlcpy(path + dir_len + 1, file, file_len + 1);
	}
	else
		ft_strlcpy(path + dir_len, file, file_len + 1);
	return (path);
}

// Vérifie si un fichier correspond à un motif en tenant compte des règles
// de filtrage (comme ignorer les fichiers cachés).
int	is_file_match(const char *filename, const char *pattern)
{
	if (should_skip_file(filename, pattern))
		return (0);
	return (match_pattern(pattern, filename));
}
