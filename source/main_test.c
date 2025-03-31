/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:00:00 by lsadikaj          #+#    #+#             */
/*   Updated: 2025/03/27 18:17:20 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Fonction pour convertir un type de token en chaîne pour l'affichage
const char	*token_type_to_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	else if (type == TOKEN_PIPE)
		return ("PIPE");
	else if (type == TOKEN_REDIRECT_IN)
		return ("REDIR_IN");
	else if (type == TOKEN_REDIRECT_OUT)
		return ("REDIR_OUT");
	else if (type == TOKEN_APPEND)
		return ("APPEND");
	else if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	else if (type == TOKEN_AND)
		return ("AND");
	else if (type == TOKEN_OR)
		return ("OR");
	else if (type == TOKEN_LPAREN)
		return ("LPAREN");
	else if (type == TOKEN_RPAREN)
		return ("RPAREN");
	return ("UNKNOWN");
}

// Fonction pour convertir un type de nœud en chaîne pour l'affichage
const char	*node_type_to_str(t_node_type type)
{
	if (type == NODE_CMD)
		return ("CMD");
	else if (type == NODE_PIPE)
		return ("PIPE");
	else if (type == NODE_REDIRECT_IN)
		return ("REDIR_IN");
	else if (type == NODE_REDIRECT_OUT)
		return ("REDIR_OUT");
	else if (type == NODE_APPEND)
		return ("APPEND");
	else if (type == NODE_HEREDOC)
		return ("HEREDOC");
	else if (type == NODE_AND)
		return ("AND");
	else if (type == NODE_OR)
		return ("OR");
	return ("UNKNOWN");
}

// Affiche la liste des tokens
void	print_tokens(t_token *tokens)
{
	int	i;

	if (!tokens)
	{
		printf("Aucun token trouvé.\n");
		return ;
	}
	printf("\n=== TOKENS LIST ===\n");
	i = 0;
	while (tokens)
	{
		printf("Token[%d]: type=%-10s value=\"%s\"\n", 
			i++, token_type_to_str(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("===================\n\n");
}

// Affiche l'AST avec indentation pour représenter la hiérarchie
void	print_ast_recursive(t_node *node, int depth)
{
	int	i;
	int	j;

	if (!node)
		return ;
	// Affichage de l'indentation
	for (i = 0; i < depth; i++)
		printf("  ");
	
	printf("Node: type=%s", node_type_to_str(node->type));
	
	if (node->type == NODE_CMD && node->cmd)
	{
		printf(" cmd=[");
		j = 0;
		while (node->cmd[j])
		{
			printf("\"%s\"", node->cmd[j]);
			if (node->cmd[j + 1])
				printf(", ");
			j++;
		}
		printf("]");
	}
	printf("\n");
	
	if (node->left)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Left:\n");
		print_ast_recursive(node->left, depth + 1);
	}
	
	if (node->right)
	{
		for (i = 0; i < depth; i++)
			printf("  ");
		printf("Right:\n");
		print_ast_recursive(node->right, depth + 1);
	}
}

// Fonction wrapper pour afficher l'AST
void	print_ast(t_node *ast)
{
	if (!ast)
	{
		printf("Arbre syntaxique vide.\n");
		return ;
	}
	printf("\n=== SYNTAX TREE ===\n");
	print_ast_recursive(ast, 0);
	printf("===================\n\n");
}

// Fonction pour tester avec une entrée donnée
void	test_parser(char *input)
{
	t_token	*tokens;
	t_node	*ast;

	printf("\n=====================================================\n");
	printf("TEST: '%s'\n", input);
	printf("=====================================================\n");
	
	// Tokenization
	tokens = tokenize(input);
	if (!tokens)
	{
		printf("Erreur de tokenization.\n");
		return ;
	}
	print_tokens(tokens);
	
	// Validation syntaxique
	if (!is_valid_syntax(tokens))
	{
		printf("Erreur de syntaxe détectée.\n");
		free_tokens(tokens);
		return ;
	}
	
	// Parsing
	ast = parse_ast(tokens);
	if (!ast)
	{
		printf("Erreur de parsing.\n");
		free_tokens(tokens);
		return ;
	}
	print_ast(ast);
	
	// Libération de la mémoire
	free_tokens(tokens);
	free_ast(ast);
}

int	main(void)
{
	printf("=== TEST DU TOKENIZER ET DU PARSER ===\n\n");
	
	// Tests simples
	test_parser("echo hello");
	test_parser("ls -la | grep .c");
	
	// Tests avec redirections
	test_parser("echo hello > file.txt");
	test_parser("cat < input.txt > output.txt");
	
	// Tests avec opérateurs logiques
	test_parser("ls && echo success");
	test_parser("ls || echo failure");
	
	// Tests avec parenthèses
	test_parser("(ls -la)");
	test_parser("(echo hello) && (ls -la)");
	test_parser("(cat file.txt || echo error) | grep pattern");
	
	// Tests complexes
	test_parser("((ls && echo ok) || (echo fail)) > result.txt");
	
	// Tests d'erreurs
	test_parser("(ls -la");  // Parenthèse non fermée
	test_parser(")ls -la");  // Parenthèse fermante sans ouvrante
	
	return (0);
}