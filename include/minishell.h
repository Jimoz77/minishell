#ifndef SHELL_H
# define SHELL_H

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
	TOKEN_HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

// Prototypes fonctions pour (tokenize)
t_token			*tokenize(char *input);
void			add_token(t_token **list, char *value, t_token_type type);
t_token_type	get_operator_type(char *str);
int				operator_length(char *str);
void			ft_getcwd(void);
void			ft_read_line(char **envp);
char 			*ft_path_finder(char *cmd);
int				ft_is_builtin(char *cmd, char **envp);
#endif
