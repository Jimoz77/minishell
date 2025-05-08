/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_read_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jimpa <jimpa@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 14:42:25 by jiparcer          #+#    #+#             */
/*   Updated: 2025/05/08 19:53:44 by jimpa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//***********************************************************************/
// Ajoutez cette fonction dans ft_read_line.c
const char *redirect_type_str(t_node_type type)
{
    const char *names[] = {
        [NODE_REDIRECT_IN] = "REDIR_IN",
        [NODE_REDIRECT_OUT] = "REDIR_OUT",
        [NODE_APPEND] = "APPEND",
        [NODE_HEREDOC] = "HEREDOC"
    };
    return names[type];
}
void debug_node(t_node *node, int depth) {
    if (!node) return;

	for (int i = 0; i < depth; i++)
        ft_printf("  ");

    // Affichage de l'entête avec la profondeur
    ft_printf("%*.s│\n%*.s└─ ", depth * 2, "", depth * 2, "");
    
    // Affichage du type de noeud
    const char *types[] = {
        [NODE_CMD] = "CMD", [NODE_PIPE] = "PIPE", 
        [NODE_REDIRECT_IN] = "REDIR_IN", [NODE_REDIRECT_OUT] = "REDIR_OUT",
        [NODE_APPEND] = "APPEND", [NODE_HEREDOC] = "HEREDOC",
        [NODE_AND] = "AND", [NODE_OR] = "OR", [NODE_PAREN] = "PAREN"
    };
    
    ft_printf("[%s] ", types[node->type]);

    // Détails spécifiques au type
    switch(node->type) {
        case NODE_CMD:
            ft_printf("args={");
            for (int i = 0; node->cmd[i]; i++)
                ft_printf("%s'%s'", (i > 0 ? ", " : ""), node->cmd[i]);
            ft_printf("}");
            break;
            
        case NODE_PIPE:
            ft_printf("LEFT → RIGHT (stdout[1] → stdin[0])");
            break;
            
        case NODE_REDIRECT_IN:
            ft_printf("STDIN ← '%s' (fd:%d)", 
                     node->right ? node->right->cmd[0] : "?", STDIN_FILENO);
            break;
            
        case NODE_REDIRECT_OUT:
            ft_printf("STDOUT → '%s' (fd:%d)", 
                     node->right ? node->right->cmd[0] : "?", STDOUT_FILENO);
            break;
            
        case NODE_HEREDOC:
            ft_printf("STDIN ← HERE (delimiter:'%s')", 
                     node->right ? node->right->cmd[0] : "?");
            break;
            
        default:
            break;
    }
    
    ft_printf("\n");

    // Exploration récursive avec indentation
    if (node->left) {
        debug_node(node->left, depth + 1);
    }
    if (node->right) {
        debug_node(node->right, depth + 1);
    }
}

void debug_pipe_flow(t_shell *shell) {
    ft_printf("\n=== PIPE FLOW DEBUG ===\n");
    ft_printf("Current FDs:\n");
    ft_printf("  STDIN  [%d] → %s\n", STDIN_FILENO, ttyname(STDIN_FILENO));
    ft_printf("  STDOUT [%d] → %s\n", STDOUT_FILENO, ttyname(STDOUT_FILENO));
    ft_printf("  STDERR [%d] → %s\n\n", STDERR_FILENO, ttyname(STDERR_FILENO));
    
    int pipe_count = 0;
    t_node *current = shell->ast;
    
    while (current) {
        if (current->type == NODE_PIPE) {
            ft_printf("PIPE #%d:\n", ++pipe_count);
            ft_printf("  LEFT SIDE:\n");
            debug_node(current->left, 2);
            ft_printf("  RIGHT SIDE:\n");
            debug_node(current->right, 2);
            ft_printf("\n");
        }
        current = current->left;
    }
    
    if (pipe_count == 0)
        ft_printf("No pipes detected in AST\n");
}

void print_tokens(t_shell *shell) {
    ft_printf("\n=== AST DEBUG ===\n");
    ft_printf("CURRENT FD STATUS:\n");
    ft_printf(" STDIN  [0] → TTY: %s\n", ttyname(STDIN_FILENO));
    ft_printf(" STDOUT [1] → TTY: %s\n", ttyname(STDOUT_FILENO));
    ft_printf(" STDERR [2] → TTY: %s\n\n", ttyname(STDERR_FILENO));
    
    debug_node(shell->ast, 0);
    
    ft_printf("\nREDIRECTION CHAIN:\n");
    t_node *current = shell->ast;
    while (current) {
        if (is_redirect_node(current->type)) {
            ft_printf("[%s] → ", redirect_type_str(current->type));
            if (current->right) {
                ft_printf("FILE: %s (FD:%d)\n", 
                        current->right->cmd[0], 
                        current->type == NODE_REDIRECT_IN ? STDIN_FILENO : STDOUT_FILENO);
            }
        }
        current = current->left;
    }
    ft_printf("\n");
}

//***************************************************************** */

// Traite l'entrée utilisateur : tokenize, parse, exécute puis libère
static void	process_input(t_shell *shell, char *input)
{
	shell->tokens = tokenize(input);
	if (!shell->tokens)
	{
		free(input);
		return ;
	}
	if (!is_valid_syntax(shell->tokens))
	{
        free_tokens(shell->tokens);
		free(input);
		return ;
	}
    scan_envar(shell);
	shell->ast = parse_ast(shell->tokens);
	if (!shell->ast)
	{
        free_tokens(shell->tokens);
		free(input);
		return ;
	}
	shell->exit_status = execute_ast(shell->ast , shell->envp);
	//free_shell(shell);
	free(input);
}

// Boucle principale de lecture de ligne
static void	ft_read_line_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		shell->tokens = NULL;
		shell->ast = NULL;
		if (g_signal == 130)
		{
			g_signal = 0;
			rl_on_new_line();
		}
		ft_getcwd();
		input = readline("minishell> ");
		if (!input)
		{
			ft_printf("\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			save_history(input);
			process_input(shell, input);
			//print_tokens(shell);
		}
		else
			free(input);
	}
}

// Fonction d'initialisation et lancement de la boucle
void	ft_read_line(char **envp)
{
	t_shell	*shell;

	shell = init_shell(envp);
	if (!shell)
		return ;
	load_history();
	ft_read_line_loop(shell);
	free_shell(shell);
}
