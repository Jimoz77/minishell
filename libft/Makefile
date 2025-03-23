# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lsadikaj <lsadikaj@student.42lausanne.ch>  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/01 14:48:22 by lsadikaj          #+#    #+#              #
#    Updated: 2025/02/12 10:13:57 by lsadikaj         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	  ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	  ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	  ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	  ft_atoi.c ft_calloc.c ft_strdup.c \
	  ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
	  ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c \
	  ft_putendl_fd.c ft_putnbr_fd.c \
	  ft_printf/ft_printf.c ft_printf/ft_print_char.c ft_printf/ft_print_str.c \
	  ft_printf/ft_print_nbr.c ft_printf/ft_print_unbr.c ft_printf/ft_print_ptr.c \
	  ft_printf/ft_print_hex.c \
	  get_next_line/get_next_line.c get_next_line/get_next_line_utils.c \
	  get_next_line/get_next_line_bonus.c \
	  get_next_line/get_next_line_utils_bonus.c

BONUS_SRC = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c \
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c \
			ft_lstmap.c

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I. -I ft_printf -I get_next_line

all : $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

bonus: $(OBJ) $(BONUS_OBJ)
	ar rcs $(NAME) $(OBJ) $(BONUS_OBJ)

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all