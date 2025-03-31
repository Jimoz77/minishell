#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>

# include "../libft/libft.h"
# include "../libft/ft_printf/ft_printf.h"
# include "../libft/get_next_line/get_next_line.h"


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

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR
}	t_node_type;

typedef struct s_node
{
	t_node_type		type;
	char			**cmd;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

// Prototypes initialisation
void			ft_getcwd(void);
void			ft_read_line(char **envp);
char			*ft_path_finder(char *cmd);
int				ft_is_builtin(char *cmd, char **envp);

// Prototypes (tokenizer)
t_token			*tokenize(char *input);
int				is_space(char c);
int				is_operator_str(char *str);
void			add_token(t_token **list, char *value, t_token_type type);
t_token			*new_token(char *value, t_token_type type);
t_token_type	get_operator_type(char *str);
int				operator_length(char *str);
int				handle_operator(t_token **tokens, char *input);
int				handle_word(t_token **tokens, char *input);
int				handle_quotes(t_token **tokens, char *input);
int				handle_parenthesis(t_token **tokens, char *input);

// Prototypes (parser)
t_node			*parse_ast(t_token *tokens);
int    			find_lowest_priority(t_token *tokens);
t_node			*create_cmd_node(t_token *tokens);
char			**fill_cmd_array(t_token *tokens, int count);
int				count_words(t_token *tokens);
t_node_type		token_to_node_type(t_token_type token_type);
t_node			*create_op_node(t_token *tokens, t_token *op);
t_node			*handle_paren_and_op(t_token *tokens);
t_token         *get_token_at(t_token *tokens, int pos);

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

#endif
