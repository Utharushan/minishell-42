# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/19 17:33:01 by tuthayak          #+#    #+#              #
#    Updated: 2024/12/19 17:33:01 by tuthayak         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	src/libft/ft_str/ft_atoi.c \
				src/libft/ft_str/ft_atof.c \
				src/libft/ft_mem/ft_bzero.c \
				src/libft/ft_mem/ft_calloc.c \
				src/libft/ft_is/ft_isalnum.c \
				src/libft/ft_is/ft_isalpha.c \
				src/libft/ft_is/ft_isascii.c \
				src/libft/ft_is/ft_isdigit.c \
				src/libft/ft_is/ft_isprint.c \
				src/libft/ft_is/ft_isspace.c \
				src/libft/ft_str/ft_itoa.c \
				src/libft/ft_mem/ft_memchr.c \
				src/libft/ft_mem/ft_memcmp.c \
				src/libft/ft_mem/ft_memcpy.c \
				src/libft/ft_mem/ft_memmove.c \
				src/libft/ft_mem/ft_memset.c \
				src/libft/ft_put/ft_putchar_fd.c \
				src/libft/ft_put/ft_putendl_fd.c \
				src/libft/ft_put/ft_putnbr_fd.c \
				src/libft/ft_put/ft_putstr_fd.c \
				src/libft/ft_str/ft_split.c \
				src/libft/ft_str/ft_strchr.c \
				src/libft/ft_str/ft_strdup.c \
				src/libft/ft_str/ft_striteri.c \
				src/libft/ft_str/ft_strjoin.c \
				src/libft/ft_str/ft_strlcat.c \
				src/libft/ft_str/ft_strlcpy.c \
				src/libft/ft_str/ft_strlen.c \
				src/libft/ft_str/ft_strmapi.c \
				src/libft/ft_str/ft_strncmp.c \
				src/libft/ft_str/ft_strnstr.c \
				src/libft/ft_str/ft_strrchr.c \
				src/libft/ft_str/ft_strtrim.c \
				src/libft/ft_str/ft_substr.c \
				src/libft/ft_is/ft_tolower.c \
				src/libft/ft_is/ft_toupper.c \
				src/libft/ft_lst/ft_lstadd_back.c \
				src/libft/ft_lst/ft_lstadd_front.c \
				src/libft/ft_lst/ft_lstclear.c \
				src/libft/ft_lst/ft_lstdelone.c \
				src/libft/ft_lst/ft_lstiter.c \
				src/libft/ft_lst/ft_lstlast.c \
				src/libft/ft_lst/ft_lstmap.c \
				src/libft/ft_lst/ft_lstnew.c \
				src/libft/ft_lst/ft_lstsize.c \
				src/ft_printf/ft_printf.c \
				src/ft_printf/ft_printf_utils.c \
				src/get_next_line/get_next_line.c
OBJS		= $(SRCS:.c=.o)

CC			= cc
RM			= rm -rf
CFLAGS		= -Wall -Wextra -Werror

NAME		= libftprintf.a

all:		$(NAME)

$(NAME):	$(OBJS)
			ar rcs $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean $(NAME)

.PHONY:		all clean fclean re
