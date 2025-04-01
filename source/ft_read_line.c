/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/03/29 15:54:51 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Fonction temporaire pour afficher l'AST à des fins de test
static void	print_ast_simple(t_node *node, int depth)
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
}

void	ft_read_line2(char *input, char ***envp)
{
	int		status;
	pid_t	pid;
	char	**cmd;
	t_token	*tokens;
	t_node	*ast;

	/* ===== DÉBUT DE LA PARTIE TOKENIZER/PARSER ===== */
	// Tokenization
	tokens = tokenize(input);
	if (!tokens)
	{
		free(input);
		return ;
	}

	// Affichage des tokens (pour débogage)
	ft_printf("\n=== TOKENS ===\n");
	t_token *tmp = tokens;
	while (tmp)
	{
		printf("Token: type=%d, value='%s'\n", tmp->type, tmp->value);
		tmp = tmp->next;
	}

	// Validation syntaxique
	if (!is_valid_syntax(tokens))
	{
		ft_printf("Erreur de syntaxe détectée\n");
		free_tokens(tokens);
		free(input);
		return ;
	}

	// Construction de l'AST
	ast = parse_ast(tokens);
	if (!ast)
	{
		ft_printf("Erreur lors de la création de l'AST\n");
		free_tokens(tokens);
		free(input);
		return ;
	}

	// Affichage de l'AST (pour débogage)
	ft_printf("\n=== ARBRE SYNTAXIQUE ===\n");
	print_ast_simple(ast, 0);
	/* ===== FIN DE LA PARTIE TOKENIZER/PARSER ===== */
	
	/* TODO: Remplacer cette partie par l'exécution basée sur l'AST quand 
	   la partie exécution sera terminée */
		=> Puis on exécutera à partir de l’arbre (et non plus avec ft_split)
	========================================================================== */
  
	cmd = ft_split(input, ' ');
	pid = fork();
	if (pid == 0)
	{	
		if ((ft_is_builtin(cmd[0], envp) == 0))
		{
			execve(ft_path_finder(input), cmd, envp);
			perror("execve");
			exit(1);
		}
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
	}
	else
		perror("fork"); 
	
	// Libération de la mémoire
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
