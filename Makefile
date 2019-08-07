# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/08/07 22:09:26 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := lem-in

SRCDIR := srcs/

SRCSFILES := main.c read_input.c validate.c build_anthill.c prepare_colony.c\
			explore_anthill.c pathfinding.c open_the_gates.c queue.c paths.c

OBJDIR := $(SRCDIR)obj/

LFTDIR := $(SRCDIR)libft/

LFT := $(LFTDIR)libft.a

LFTPRINTFDIR := $(SRCDIR)ft_printf/

LFTPRINTF := $(LFTPRINTFDIR)libftprintf.a

SRCS := $(addprefix $(SRCDIR), $(SRCSFILES))

OBJ := $(SRCS:%.c=$(OBJDIR)%.o)

CC := gcc -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(LFT) $(LFTPRINTF) $(OBJ)
	@$(CC) $^ -o $@
	@printf "\r\e[J\e[32m$@\e[0m done!\n\e[?25h"

$(LFT):
	@$(MAKE) -C $(LFTDIR)
	@$(MAKE) -C $(LFTDIR) clean

$(LFTPRINTF):
	@$(MAKE) -C $(LFTPRINTFDIR)
	@$(MAKE) -C $(LFTPRINTFDIR) clean

$(OBJDIR)%.o: %.c
	@mkdir -p '$(@D)'
	@$(CC) -I./include -I./$(LFTDIR) -c $< -o $@
	@printf "\r\e[?25l\e[Jcompiling \e[32m$(notdir $^)\e[0m"

clean:
	@rm -rf $(OBJDIR)
	@$(MAKE) -C $(LFTDIR) fclean
	@$(MAKE) -C $(LFTPRINTFDIR) fclean

fclean: clean
	@rm -f $(NAME)

re: fclean all
