/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:35:19 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/05/05 19:03:39 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Lit l'entrée jusqu'au délimiteur et renvoie le contenu */
static char	*read_heredoc_input(const char *delimiter)
{
	char	*line;
	char	*content;
	char	*tmp;

	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		line = readline("> ");
		if (!line || !*line)
		{
			free(line);
			break ;
		}
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		tmp = ft_strjoin(content, line);
		free(content);
		free(line);
		content = ft_strjoin(tmp, "\n");
		free(tmp);
		if (!content)
			return (NULL);
	}
	return (content);
}

/* Traite un heredoc et stocke son contenu */
int	process_single_heredoc(t_node *node, int index, t_shell *shell)
{
	char	*delimiter;
	char	*content;
	char	*tmp;

	delimiter = get_unquoted_filename(node);
	if (!delimiter)
		return (-1);
	content = read_heredoc_input(delimiter);
	free(delimiter);
	if (!content)
		return (-1);
	if (content[0] != '\0' && content[ft_strlen(content) - 1] != '\n')
	{
		tmp = ft_strjoin(content, "\n");
		free(content);
		content = tmp;
		if (!content)
			return (-1);
	}
	shell->heredocs[index] = content;
	return (0);
}

/* Collecte les heredocs dans l'ordre de la commande originale */
void	collect_all_heredocs(t_node *node, t_node **heredocs, int *count)
{
	t_node	*stack[100];
	int		top;
	int		i;

	top = 0;
	while (node && is_redirect_node(node->type))
	{
		if (node->type == NODE_HEREDOC)
			stack[top++] = node;
		node = node->left;
	}
	i = top - 1;
	while (i >= 0)
	{
		heredocs[(*count)++] = stack[i];
		i--;
	}
}
