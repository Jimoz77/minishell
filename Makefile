# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/25 14:00:24 by lsadikaj          #+#    #+#              #
#    Updated: 2025/06/10 17:43:59 by lsadikaj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom de l'exécutable
NAME = minishell

# Compilateur et options
CC = cc
CFLAGS = -O0 -g3 -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Dossiers
SRC_DIR = source
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Fichiers source
SRCS = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/ft_getcwd.c \
	$(SRC_DIR)/ft_read_line.c \
	$(SRC_DIR)/ft_read_line_utils.c \
	$(SRC_DIR)/signals.c \
	$(SRC_DIR)/built-in/ft_cd.c \
	$(SRC_DIR)/built-in/ft_echo.c \
	$(SRC_DIR)/built-in/ft_env.c \
	$(SRC_DIR)/built-in/ft_export.c \
	$(SRC_DIR)/built-in/ft_is_builtin.c \
	$(SRC_DIR)/built-in/ft_pwd.c \
	$(SRC_DIR)/built-in/ft_unset.c \
	$(SRC_DIR)/tokenizer/tokenizer.c \
	$(SRC_DIR)/tokenizer/tokenizer_utils.c \
	$(SRC_DIR)/tokenizer/tokenizer_handle.c \
	$(SRC_DIR)/tokenizer/tokenizer_quotes.c \
	$(SRC_DIR)/tokenizer/tokenizer_quotes_helper.c \
	$(SRC_DIR)/tokenizer/tokenizer_quotes_utils.c \
	$(SRC_DIR)/tokenizer/envar_to_value.c \
	$(SRC_DIR)/parser/parser.c \
	$(SRC_DIR)/parser/parser_utils.c \
	$(SRC_DIR)/parser/parser_utils2.c \
	$(SRC_DIR)/parser/parser_utils2_helper.c \
	$(SRC_DIR)/parser/parser_utils3.c \
	$(SRC_DIR)/parser/parser_ops.c \
	$(SRC_DIR)/parser/parser_ops_helper1.c \
	$(SRC_DIR)/parser/parser_ops_helper2.c \
	$(SRC_DIR)/parser/parser_ops_helper3.c \
	$(SRC_DIR)/parser/parser_ops2.c \
	$(SRC_DIR)/parser/parser_syntax.c \
	$(SRC_DIR)/parser/parser_syntax_separate.c \
	$(SRC_DIR)/parser/parser_syntax2.c \
	$(SRC_DIR)/utils/memory_utils.c \
	$(SRC_DIR)/utils/memory_utils2.c \
	$(SRC_DIR)/utils/ft_array_dup.c \
	$(SRC_DIR)/utils/ft_path_finder.c \
	$(SRC_DIR)/utils/shell_utils.c \
	$(SRC_DIR)/utils/handle_unclosed_quotes.c \
	$(SRC_DIR)/executor/executor.c \
	$(SRC_DIR)/executor/executor_cmd.c \
	$(SRC_DIR)/executor/executor_cmd_utils.c \
	$(SRC_DIR)/executor/executor_cmd_utils2.c \
	$(SRC_DIR)/executor/executor_cmd_utils3.c \
	$(SRC_DIR)/executor/executor_heredoc.c \
	$(SRC_DIR)/executor/executor_heredoc_utils.c \
	$(SRC_DIR)/executor/executor_redir.c \
	$(SRC_DIR)/executor/executor_redir2.c \
	$(SRC_DIR)/executor/executor_paren.c \
	$(SRC_DIR)/executor/executor_pipe.c \
	$(SRC_DIR)/executor/executor_pipe_utils.c \
	$(SRC_DIR)/utils/save_env.c \
	$(SRC_DIR)/wildcards/wildcards.c \
	$(SRC_DIR)/wildcards/wildcards_utils.c \
	$(SRC_DIR)/wildcards/wildcards_utils2.c \
	$(SRC_DIR)/wildcards/wildcards_matching.c \
	$(SRC_DIR)/wildcards/wildcards_helpers.c

# Fichiers objets
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Compilation des fichiers objets dans les sous-dossiers
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compilation de l'exécutable
$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

# Règle principale
all: $(NAME)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
