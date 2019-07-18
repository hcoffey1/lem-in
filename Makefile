# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/07/18 23:26:42 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

all: $(NAME)

$(NAME):
	gcc -g -I./include srcs/*.c -L./srcs/libft -lft -o $(NAME)

fclean:
	rm -rf $(NAME)

re: fclean all
