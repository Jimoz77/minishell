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
# include <sys/stat.h>
# include <errno.h>
# include <dirent.h>

# include "../libft/libft.h"
# include "../libft/ft_printf/ft_printf.h"
# include "../libft/get_next_line/get_next_line.h"

typedef struct s_printsort
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
}	t_printsort;

// Structure pour les types de quotes
typedef enum s_quote_type
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE,
}	t_quote_type;

// Structure représentant une partie de mot avec quote
typedef struct s_word_part
{
	char				*content;
	t_quote_type		type;
	struct s_word_part	*next;	
}	t_word_part;

// Enum pour les types de tokens
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
	TOKEN_RPAREN,
	TOKEN_SPACE
}	t_token_type;

// Structure représentant un token
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	t_word_part		*parts;
	struct s_token	*next;
}	t_token;

// Enum pour les types de nœuds dans l'AST
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

// Forward declarations
typedef struct s_heredoc	t_heredoc;
typedef struct s_redir		t_redir;

// Structure d'un nœud dans l'AST
typedef struct s_node
{
	t_node_type		type;
	char			**cmd;
	t_heredoc		*heredocs;
	t_redir			*redirections;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

// Structure pour un heredoc
typedef struct s_heredoc
{
	char				*delimiter;
	char				*content;
	int					processed;
	int					pipe_fd;
	struct s_heredoc	*next;
}	t_heredoc;

// Structure pour une redirection simple
typedef struct s_redir
{
	t_token_type		type;
	char				*filename;
	struct s_redir		*next;
}	t_redir;

// Structure contenant les fds de redirection
typedef struct s_redirect
{
	int	stdin_fd;
	int	stdout_fd;
	int	saved_stdin;
	int	saved_stdout;
}	t_redirect;

// Structure principale du shell
typedef struct s_shell
{
	char		***envp;
	t_token		*tokens;
	t_node		*ast;
	int			exit_status;
	char		*current_dir;
}	t_shell;

// signals/
extern int	g_signal;
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

// shell_utils.c
t_shell	*init_shell(char **envp);
void	free_shell(t_shell *shell);
void	ft_read_line(char **envp);
char	*ft_path_finder(char *cmd, char ***envp);
void	free_array(char **array);
char	**load_env(void);
char	**ft_array_dup(char **array);
void	free_tokens(t_token *tokens);
void	free_redirections(t_redir *redirections);
void	free_heredocs(t_heredoc *heredocs);
void 	save_env(char ***env);
void	ft_free_split(char **split_array);
char	*ft_strjoin_free(char *s1, const char *s2);
void	free_ast(t_node *node);
void	load_history(void);
void	save_history(char *cmd);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char	*handle_unclosed_quotes(char *input);
void	init_loop_vars(t_shell *shell);
void	handle_ast_execution(t_shell *shell, char *input);
void	free_token(t_token *token);
int		get_shell_pid(void);


// tokenizer/
t_token			*tokenize(char *input);
void			add_token(t_token **list, char *value, t_token_type type);
int				is_space(char c);
int				is_operator_str(char *str);
t_token_type	get_operator_type(char *str);
int				operator_length(char *str);
int				handle_operator(t_token **tokens, char *input);
int				handle_word(t_token **tokens, char *input);
int				handle_quotes(t_token **tokens, char *input);
int				handle_parenthesis(t_token **tokens, char *input);
int				handle_complex_word(t_token **tokens, char *input);
int				parse_quoted_part(char *input, int *i, t_word_part **parts, t_quote_type type);
void			add_word_part(t_word_part **parts, char *content, t_quote_type type);
void			free_word_parts(t_word_part *parts);
char			*build_unquoted_value(t_word_part *parts);
int				scan_envar(t_shell *shell);
t_token			*delete_token(t_token *head, t_token *target);
void			envar_to_value(t_shell *shell, t_token *token);
int				scan_envar_parsing_phase(t_shell *shell);
int				scan_envar_execution_phase(t_shell *shell, t_token *tokens);
t_token			*create_complex_token(char *input, int len, t_word_part *parts);
t_token			*create_empty_token(t_word_part *parts);
int				clean_complex_word(t_word_part *parts, char *value, int ret);
void			add_new_token(t_token **tokens, t_token *new_token);

// parser/
t_node		*parse_ast(t_token *tokens);
int			find_lowest_priority(t_token *tokens);
t_node		*create_cmd_node(t_token *tokens);
char		**fill_cmd_array(t_token *tokens, int count);
int			count_words(t_token *tokens);
t_node		*create_op_node(t_token *tokens, t_token *op);
t_node		*handle_paren_and_op(t_token *tokens);
t_token		*get_token_at(t_token *tokens, int pos);
t_node_type	token_to_node_type(t_token_type token_type);
t_node		*parse_command_with_redirections(t_token **tokens);
t_token		*skip_parentheses_block(t_token *start);
int			is_inside_parentheses(t_token *tokens, t_token *target);
t_token		*find_token_before(t_token *start, t_token *target);
t_node		*handle_op_after_paren(t_node *inner_node, t_token *inner_start, t_token *op_token);
t_token		*find_command_token(t_token *tokens);
t_word_part	*duplicate_word_parts(t_word_part *parts);
t_token		*duplicate_tokens(t_token *tokens);
t_token		*duplicate_tokens_until_pos(t_token *tokens, int pos);
t_token		*find_matching_paren_public(t_token *start);
t_node		*handle_paren_and_op(t_token *tokens);
t_node		*handle_op_after_paren(t_node *inner_node, t_token *inner_start, t_token *op_token);
t_token		*create_token_until_pos(t_token *current, int pos, int i);
int			check_paren_content(t_token *start, t_token *end);
void		process_heredoc_redirection(t_token *current, t_node *cmd_node);
void	process_other_redirection(t_token *current, t_node *cmd_node);

// parser/syntax
int		is_operator(t_token_type type);
int		is_redirection(t_token_type type);
int		is_valid_syntax(t_token *tokens);
int		check_paren_balance(t_token *tokens);
int		check_start_operator(t_token *tokens);
int		check_redirections(t_token *tokens);
int		check_end_operator(t_token *tokens);
int		check_consecutive_operators(t_token *tokens);
int		check_parentheses_usage(t_token *tokens);

// executor/
int		execute_node_by_type(t_node *node, char ***envp, t_shell *shell);
int		execute_ast(t_node *node, char ***envp, t_shell *shell);
int		execute_cmd_node(t_node *node, char ***envp, t_shell *shell);
int		execute_paren_node(t_node *node, char ***envp, t_shell *shell);
int		string_to_fd(const char *content);
void	init_redirect(t_redirect *red);
void	close_redirect_fds(t_redirect *red);
void	restore_std_fds(t_redirect *red);
t_node	*find_command_node(t_node *node);
int		is_redirect_node(t_node_type type);
int		execute_pipe_node(t_node *node, char ***envp, t_shell *shell);
int		execute_and_node(t_node *node, char ***envp, t_shell *shell);
int		execute_or_node(t_node *node, char ***envp, t_shell *shell);
int		is_directory(char *path);
char	*handle_direct_path(char *cmd);
int		handle_special_commands(char **cmd);
int		execute_with_path(char *path, char **cmd, char **envp);
int		process_cmd_tokens(t_node *node, char ***envp, t_shell *shell);
int		exec_builtin_with_redirections(t_node *node, char ***envp);
//int		exec_external(char **cmd, char **envp, t_shell *shell);
int		exec_cmd_with_redirections(t_node *node, char **envp, t_shell *shell);
int		execute_cmd_builtin_or_exec(t_node *node, char ***envp, t_redirect *red);
int		prepare_cmd_tokens(t_node *node, t_shell *shell);


// executor/redir
void	add_redirection(t_node *node, t_token_type type, char *filename);
void	add_heredoc(t_node *node, char *delimiter);
int		apply_node_redirections(t_node *node, t_redirect *red);
void	process_all_heredocs(t_node *node);
int		apply_heredoc_redir(t_node *node, char *delimiter, t_redirect *red);

// executor/cmd_utils2
t_token	*create_tokens_from_cmd(char **cmd, t_shell *shell);
void	init_token_from_cmd(t_token *token, char *cmd_arg, t_shell *shell);
void	update_cmd_from_tokens(char **cmd, t_token *tokens);
void	process_token_expansion(t_shell *shell, t_token *temp_tokens);

// BUILTINS
int		ft_is_builtin(char **cmd, char ***envp);
int		execute_builtin(char **cmd, char ***envp);
int		ft_cd(char **cmd, char ***envp);
int		ft_pwd(void);
int		ft_echo(char **cmd);
int		ft_env(char ***envp);
int		ft_export(char **cmd, char ***envp);
int		ft_unset(char **cmd, char ***envp);
void	ft_getcwd(void);
char	**ft_array_dup(char **array);
char	***ft_array_dup2(char ***array);
char	***ft_wrap_array(char **array);
int		get_envp_len(char **envp);
char	**ft_array_add(char **array, const char *str);
int		print_sorted_env(char **envp);
int		is_valid_id(char *str);

// wildcards/
void    expand_wildcards(t_token *tokens);
int     contains_wildcard(const char *str);
int     match_pattern(const char *pattern, const char *string);
int     count_matching_files(const char *pattern);
char    **get_matching_files(const char *pattern, int count);
void    sort_strings(char **strings, int count);
t_token *replace_with_matches(t_token *token, char **matches, int count);
void    process_wildcard_matches(t_token *current, t_token *prev, 
                                char ***matches_ptr, int count);
void    update_tokens_list(t_token **tokens, t_token *prev, t_token *new_token);
int     is_hidden_file(const char *filename, const char *pattern);
int     should_skip_file(const char *filename, const char *pattern);
char    *join_path(const char *dir, const char *file);
int     is_file_match(const char *filename, const char *pattern);
void    free_matches(char **matches, int count);
void    free_token_content(t_token *token);
t_token *create_match_token(char *value);
void    add_token_to_list(t_token **first, t_token *new);
int		add_if_match(char **matches, int i, const char *pattern, struct dirent *entry);
char	**free_matches_partial(char **matches, int i, DIR *dir);

#endif