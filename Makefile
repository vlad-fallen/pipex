# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbutter <mbutter@student.21-school.ru>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/04 15:40:50 by mbutter           #+#    #+#              #
#    Updated: 2021/12/10 19:39:53 by mbutter          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS		=	pipex.c

OBJS		=	$(SRCS:.c=.o)

BONUS		=	

BONUS_OBJS	=	$(BONUS:.c=.o)

D_FILES		=	$(patsubst %.c,%.d,$(SRCS)) 

D_FILES_B	=	$(patsubst %.c,%.d,$(BONUS))

CC			=	gcc

RM			=	rm -f

CFLAGS		=	-Wall -Wextra -Werror -I ./

NAME		=	pipex


.c.o:
			$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -MD

$(NAME):	$(OBJS)
			@make -C ./libft
			$(CC) $(CFLAGS) $(SRCS) ./libft/libft.a -o $(NAME)

all:		$(NAME)

clean:
			$(RM) $(OBJS) $(D_FILES) $(BONUS_OBJS)
			@make clean -C ./libft

fclean:		clean
			$(RM) $(NAME)
			@make fclean -C ./libft

re:			fclean $(NAME)

bonus:

.PHONY:		all clean fclean re bonus

include $(wildcard $(D_FILES))