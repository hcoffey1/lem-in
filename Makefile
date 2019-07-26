# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/07/26 14:44:37 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = lem-in

all: $(NAME)

$(NAME):
	gcc -g -I./include srcs/*.c -L./srcs/libft -lft -L./srcs/ft_printf -lftprintf -L./srcs/minilibx -lmlx -framework OpenGL -framework AppKit -o $(NAME)

fclean:
	rm -rf $(NAME)

re: fclean all
