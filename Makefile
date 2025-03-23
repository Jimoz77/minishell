# Nom de l'exécutable
NAME = minishell

# Compilateur et options
CC = cc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -lreadline

# Dossiers
SRC_DIR = source
OBJ_DIR = obj
INC_DIR = include
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Fichiers sources et objets
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Options d'inclusion de headers
INCLUDES = -I$(INC_DIR)

# Règle par défaut
all: $(NAME)

# Compilation de l'exécutable
$(NAME): $(OBJS)
	make -C $(LIBFT_DIR)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

# Compilation des objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Nettoyage
clean:
	rm -rf $(OBJ_DIR)
	make clean -C $(LIBFT_DIR)

fclean: clean
	rm -f $(NAME)
	make fclean -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
