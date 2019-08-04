# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/08/04 23:10:01 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

all: $(NAME)

$(NAME):
	gcc -g -I./include srcs/*.c -I./srcs/libft -L./srcs/libft -lft -L./srcs/ft_printf -lftprintf -o $(NAME)

fclean:
	rm -rf $(NAME)

re: fclean all
