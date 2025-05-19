# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/06 16:04:15 by tuthayak          #+#    #+#              #
#    Updated: 2025/04/06 16:04:15 by tuthayak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -Iincludes -Ilibftprintf/inc
LIBFTPRINTF	= libftprintf/libftprintf.a
LIBFTPRINTF_DIR	= libftprintf

SRC_DIR			= src
OBJ_DIR			= obj_test
BUILTINS_DIR	= builtins

SRCS		= $(SRC_DIR)/lexer/lexer.c \
			  $(SRC_DIR)/lexer/lexer_utils.c \
			  $(SRC_DIR)/parser/parser.c \
			  $(SRC_DIR)/parser/syntax_error_handling.c \
			  $(SRC_DIR)/test_main.c \
			  $(SRC_DIR)/exec/exec_command.c \
			  $(SRC_DIR)/exec/run_pipe.c \
			  $(SRC_DIR)/exec/redirections.c \
			  $(SRC_DIR)/exec/here_document.c \
			  $(BUILTINS_DIR)/echo.c \
			  $(BUILTINS_DIR)/pwd.c \
			  $(BUILTINS_DIR)/exit.c

OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(LIBFTPRINTF):
	@make -sC $(LIBFTPRINTF_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(LIBFTPRINTF) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFTPRINTF) -lreadline -o $(NAME)

clean:
	@rm -rf $(OBJ_DIR)
	@make clean -sC $(LIBFTPRINTF_DIR)

fclean: clean
	@rm -f $(NAME)
	@make fclean -sC $(LIBFTPRINTF_DIR)

re: fclean all

.PHONY: all clean fclean re