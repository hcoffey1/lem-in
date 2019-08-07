# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/08/07 20:15:08 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := lem-in

SRCDIR := srcs/

SRCSFILES := main.c read_input.c validate.c prepare_colony.c explore_anthill.c pathfinding.c queue.c paths.c

OBJDIR := $(SRCDIR)obj/

LFTDIR := $(SRCDIR)libft/

LFT := $(LFTDIR)libft.a

LFTPRINTFDIR := $(SRCDIR)ft_printf/

LFTPRINTF := $(LFTPRINTFDIR)libftprintf.a

SRCS := $(addprefix $(SRCDIR), $(SRCSFILES))

OBJ = $(SRCS:%.c=$(OBJDIR)%.o)

CC := gcc -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJ) $(LFT) $(LFTPRINTF)
	@$(CC) -L$(LFTDIR) -lft -L$(LFTPRINTFDIR) -lftprintf $^ -o $@
	@echo "$(NAME) done!"

$(LFT):
	@$(MAKE) -C $(LFTDIR)
	@$(MAKE) -C $(LFTDIR) clean

$(LFTPRINTF):
	@$(MAKE) -C $(LFTPRINTFDIR)
	@$(MAKE) -C $(LFTPRINTFDIR) clean

$(OBJDIR)%.o: %.c
	@mkdir -p '$(@D)'
	@$(CC) -I./include -c $< -o $@

clean:
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LFTDIR) fclean
	@$(MAKE) -C $(LFTPRINTFDIR) fclean

fclean: clean
	@rm -f $(NAME1) $(NAME2)

re: fclean all