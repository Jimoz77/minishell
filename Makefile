# Fichiers sources
SRCS	=	./source/ft_split.c ./source/ft_strjoin.c
MAIN	=	./source/minishell_1try.c

# Répertoire pour les fichiers objets
OBJ_DIR =	obj

# Fichiers objets
OBJS	=	$(SRCS:./source/%.c=$(OBJ_DIR)/%.o)

# Compilateur et options
CC		=	gcc
FLAGS	=	-lreadline
NAME	=	minishell

# Règle par défaut
all:		$(NAME)

# Règle pour créer l'exécutable
$(NAME):	$(OBJS)
			$(CC) $(MAIN) $(OBJS) $(FLAGS) -o $(NAME)

# Règle pour compiler les fichiers sources en fichiers objets dans le répertoire obj
$(OBJ_DIR)/%.o: ./source/%.c
			@mkdir -p $(OBJ_DIR)
			$(CC) -c $< -o $@

# Règle pour nettoyer les fichiers objets
clean:
			rm -rf $(OBJ_DIR)

# Règle pour nettoyer les fichiers objets et l'exécutable
fclean:		clean
			rm -f $(NAME)

# Règle pour recompiler tout
re:			fclean all

.PHONY:		all clean fclean re