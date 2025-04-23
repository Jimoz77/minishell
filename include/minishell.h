/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 19:55:59 by jimpa             #+#    #+#             */
/*   Updated: 2025/04/23 18:35:15 by lsadikaj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <signal.h>
# include <ctype.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <fcntl.h>

# include "../libft/libft.h"
# include "../libft/ft_printf/ft_printf.h"
# include "../libft/get_next_line/get_next_line.h"

// Type de mots
typedef enum s_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE	
}	t_quote_type;

// Structure pour stocker les parties d'un mot
typedef struct s_word_part
{
	char				*content;
	t_quote_type		type;
	struct s_word_part	*next;	
}	t_word_part;

// Enum pour les types de token
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_LPAREN,
	TOKEN_RPAREN
}	t_token_type;

// Structure pour les tokens
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_word_part		*parts;
	struct s_token	*next;
}	t_token;

// Enum pour les types de noeud
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_PAREN,
}	t_node_type;

// Structure pour l'AST
typedef struct s_node
{
	t_node_type		type;
	char			**cmd;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

// Structure pour stocker les redirections
typedef struct s_redirect
{
	int	stdin_fd;
	int	stdout_fd;
	int	saved_stdin;
	int	saved_stdout;
}	t_redirect;

// Structure pour l'environnement du shell
typedef struct s_shell
{
	char		***envp;
	t_token		*tokens;
	t_node		*ast;
	int			exit_status;
	char		*current_dir;
}	t_shell;

// Signaux 
extern int	g_signal;
void			setup_signals(void);
void			handle_sigint(int sig);
void			handle_sigquit(int sig);

// Prototypes initialisation (utils)
void			ft_read_line(char **envp);
t_shell			*init_shell(char **envp);
void			free_shell(t_shell *shell);
char			*ft_path_finder(char *cmd, char ***envp);
void			free_array(char **array);

// Prototypes (tokenizer)
t_token			*tokenize(char *input);
int				is_space(char c);
int				is_operator_str(char *str);
void			add_token(t_token **list, char *value, t_token_type type);
t_token_type	get_operator_type(char *str);
int				operator_length(char *str);
int				handle_operator(t_token **tokens, char *input);
int				handle_word(t_token **tokens, char *input);
int				handle_quotes(t_token **tokens, char *input);
int				handle_parenthesis(t_token **tokens, char *input);
int				parse_quoted_part(char *input, int *i, t_word_part **parts,
					t_quote_type type);
int				handle_complex_word(t_token **tokens, char *input);
void			free_word_parts(t_word_part *parts);
char			*build_unquoted_value(t_word_part *parts);
char			*get_unquoted_filename(t_node *node);
int				scan_envar(t_token *tokens, char ***envp);
void			envar_to_value(char ***envp, t_token *tokens);

// Prototypes (parser)
t_node			*parse_ast(t_token *tokens);
int				find_lowest_priority(t_token *tokens);
t_node			*create_cmd_node(t_token *tokens);
char			**fill_cmd_array(t_token *tokens, int count);
int				count_words(t_token *tokens);
t_node_type		token_to_node_type(t_token_type token_type);
t_node			*setup_redirect_left(t_token *tokens);
t_node			*create_redirect_right(t_token *right_part);
t_node			*create_op_node(t_token *tokens, t_token *op);
t_node			*handle_paren_and_op(t_token *tokens);
t_token			*get_token_at(t_token *tokens, int pos);
void			add_word_part(t_word_part **parts, char *content,
					t_quote_type type);
t_node			*setup_redirect_node(t_node *node, t_token *tokens,
					t_token *right_part);

// Prototypes (parser parenthèses)
t_token			*find_matching_paren(t_token *start);
t_token			*clone_tokens(t_token *start, t_token *end);
t_node			*parse_parenthesized_expr(t_token *tokens);

// Prototypes de validation de syntaxe
int				is_operator(t_token_type type);
int				is_redirection(t_token_type type);
int				check_paren_balance(t_token *tokens);
int				is_valid_syntax(t_token *tokens);
int				check_paren_balance(t_token *tokens);
int				check_start_operator(t_token *tokens);
int				check_redirections(t_token *tokens);
int				check_end_operator(t_token *tokens);
int				check_consecutive_operators(t_token *tokens);
int				check_parentheses_usage(t_token *tokens);

// Prototypes de gestion de mémoire (utils)
void			free_tokens(t_token *tokens);
void			free_ast(t_node *node);
char			*ft_strjoin_free(char *s1, const char *s2);
void			ft_free_split(char **split_array);
void			save_env(char ***env);
char			**load_env(void);

// Prototypes (built-in)
void			ft_getcwd(void);
int				ft_is_builtin(char **cmd, char ***envp);
int				ft_cd(char **cmd, char ***envp);
int				ft_pwd(void);
int				ft_echo(char **cmd);
int				ft_env(char ***envp);
int				ft_export(char **cmd, char ***envp);
char			**ft_array_dup(char **array);
char			***ft_array_dup2(char ***array);
char			***ft_wrap_array(char **array);
int				is_valid_id(char *str);
int				ft_unset(char **cmd, char ***envp);
void			execute_builtin(char **cmd, char ***envp);

// Prototype (executor)
int				execute_ast(t_node *node, char ***envp);
int				is_redirect_node(t_node_type type);
int				setup_all_redirects(t_node *node, t_redirect *red);
int				execute_combined_node(t_node *node, char ***envp);
int				execute_cmd_node(t_node *node, char ***envp);
int				execute_redirect_node(t_node *node, char ***envp);
int				handle_heredoc(t_node *node, char ***envp);
int				process_single_heredoc(t_node *node);
void			read_heredoc_input(int pipe_fd, char *delimiter);
int				handle_heredoc_error(int *pipe_fd, char *error_msg);
int				handle_heredoc_status(int status);
int				execute_paren_node(t_node *node, char ***envp);
int				collect_redirects(t_node *node, t_node **redirections);
int				process_heredoc_node(t_node *current, t_redirect *red);
int				process_redirect_in(t_node *current, t_redirect *red);
int				process_redirect_out(t_node *current, t_redirect *red);
int				process_redirect_node(t_node *node, t_redirect *red,
					int *in, int *out);

#endif
