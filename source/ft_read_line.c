/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/04/17 20:11:25 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Fonction temporaire pour afficher l'AST à des fins de test
/* static void	print_ast_simple(t_node *node, int depth)
{
	int	i;
	
	if (!node)
		return ;
	
	for (i = 0; i < depth; i++)
		ft_printf("  ");
	
	ft_printf("Node type: ");
	if (node->type == NODE_CMD)
		ft_printf("CMD");
	else if (node->type == NODE_PIPE)
		ft_printf("PIPE");
	else if (node->type == NODE_REDIRECT_IN)
		ft_printf("REDIRECT_IN");
	else if (node->type == NODE_REDIRECT_OUT)
		ft_printf("REDIRECT_OUT");
	else if (node->type == NODE_APPEND)
		ft_printf("APPEND");
	else if (node->type == NODE_HEREDOC)
		ft_printf("HEREDOC");
	else if (node->type == NODE_AND)
		ft_printf("AND");
	else if (node->type == NODE_OR)
		ft_printf("OR");
	
	if (node->type == NODE_CMD && node->cmd)
	{
		ft_printf(" - Commande: ");
		i = 0;
		while (node->cmd[i])
		{
			ft_printf("'%s' ", node->cmd[i]);
			i++;
		}
	}
	ft_printf("\n");
	
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			ft_printf("  ");
		ft_printf("Left:\n");
		print_ast_simple(node->left, depth + 1);
	}
	
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			ft_printf("  ");
		ft_printf("Right:\n");
		print_ast_simple(node->right, depth + 1);
	}
} */

void	ft_read_line2(char *input, char ***envp)
{
	int		status;
	pid_t	pid;
	t_token	*tokens;
	t_node	*ast;

	/* ===== PARTIE TOKENIZER/PARSER ===== */
	tokens = tokenize(input);
	if (!tokens)
		return (free(input));

	if (!is_valid_syntax(tokens))
		return (free_tokens(tokens), free(input));

	scan_envar(tokens, envp);
	ast = parse_ast(tokens);
	if (!ast)
		return (free_tokens(tokens), free(input));

	/* ===== PARTIE EXÉCUTION ===== */
	if (ast->type == NODE_CMD && ast->cmd && ast->cmd[0])
	{
		// Gestion des builtins dans le processus parent
		if (ft_is_builtin(ast->cmd, envp))
		{
			execute_builtin(ast->cmd, envp);
		}
		else
		{
			pid = fork();
			if (pid == 0)
			{
				char *path = ft_path_finder(ast->cmd[0], envp);
				if (path)
				{
					execve(path, ast->cmd, *envp);
					free(path);
				}
				perror(ast->cmd[0]);
				exit(1);
			}
			else if (pid > 0)
				waitpid(pid, &status, 0);
			else
				perror("fork");
		}
	}

	/* ===== NETTOYAGE ===== */
	free_tokens(tokens);
	free_ast(ast);
	free(input);
}


void	ft_read_line(char ***envp)
{
	char	*input;
	
	read_history(".minishell_history");
	
	while (1)
	{
		ft_getcwd();
		input = readline("minishell> ");
		if (!input)
		{
			printf("\n");
			break;
		}
		if (*input)
		{
			add_history(input);
			ft_read_line2(input, envp);
		}
	}
	// Sauvegarder l'historique à la fin de la session
	write_history(".minishell_history");
}
