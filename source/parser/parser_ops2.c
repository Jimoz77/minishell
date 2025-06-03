/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ops2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:35:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/06/03 19:34:57 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Prépare le contenu de la commande de redirection
static char	**prepare_redirect_cmd(t_token *right_part)
{
	char	**cmd;
	char	*value;/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   parser_ops2.c                                      :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2025/05/29 17:45:00 by lsadikaj          #+#    #+#             */
	/*   Updated: 2025/06/03 14:39:11 by lsadikaj         ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */
	
	#include "../../include/minishell.h"
	
	t_token	*find_token_before(t_token *start, t_token *target)
	{
		t_token	*current;
		t_token	*prev;
	
		if (!start || !target || start == target)
			return (NULL);
		current = start;
		prev = NULL;
		while (current && current != target)
		{
			prev = current;
			current = current->next;
		}
		return (prev);
	}
	
	t_node	*handle_op_after_paren(t_node *inner_node, t_token *op_token)
	{
		t_node	*op_node;
	
		if (!inner_node || !op_token)
			return (inner_node);
		op_node = ft_calloc(1, sizeof(t_node));
		if (!op_node)
		{
			free_ast(inner_node);
			return (NULL);
		}
		op_node->type = token_to_node_type(op_token->type);
		op_node->left = inner_node;
		if (op_token->next)
			op_node->right = parse_ast(op_token->next);
		else
			op_node->right = NULL;
		if (!op_node->right && op_node->type != NODE_CMD)
		{
			free_ast(op_node);
			return (NULL);
		}
		return (op_node);
	}

	if (right_part->parts)
		value = build_unquoted_value(right_part->parts);
	else
		value = ft_strdup(right_part->value);
	if (!value)
		return (NULL);
	cmd = malloc(sizeof(char *) * 2);
	if (!cmd)
	{
		free(value);
		return (NULL);
	}
	cmd[0] = value;
	cmd[1] = NULL;
	return (cmd);
}

// Crée un nœud pour la partie droite d'une redirection
t_node	*create_redirect_right(t_token *right_part)
{
	t_node	*right;
	char	**cmd;

	if (!right_part)
		return (NULL);
	right = malloc(sizeof(t_node));
	if (!right)
		return (NULL);
	right->type = NODE_CMD;
	right->left = NULL;
	right->right = NULL;
	cmd = prepare_redirect_cmd(right_part);
	if (!cmd)
	{
		free(right);
		return (NULL);
	}
	right->cmd = cmd;
	return (right);
}

// Configure le nœud de redirection avec ses parties gauche et droite
t_node	*setup_redirect_node(t_node *node, t_token *tokens,
							t_token *right_part)
{
	node->right = create_redirect_right(right_part);
	if (!node->right)
	{
		free(node);
		return (NULL);
	}
	node->left = setup_redirect_left(tokens);
	return (node);
}

// Crée le sous-arbre gauche d’un nœud de redirection
t_node	*setup_redirect_left(t_token *tokens)
{
	if (!tokens || is_redirection(tokens->type))
		return (NULL);
	if (tokens->type == TOKEN_WORD && !tokens->next)
		return (create_cmd_node(tokens));
	else
		return (parse_ast(tokens));
}

// Renvoie le token situé à une position donnée dans la liste
t_token	*get_token_at(t_token *tokens, int pos)
{
	t_token	*tmp = tokens;
	int		i = 0;

	while (tmp && i < pos)
	{
		tmp = tmp->next;
		i++;
	}
	return (tmp);
}
