# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smorty <smorty@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/18 19:46:57 by smorty            #+#    #+#              #
#    Updated: 2019/09/03 23:42:46 by smorty           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := lem-in

SRCSFILES := main.c store_file.c read_input.c validate.c error.c\
			build_anthill.c prepare_colony.c explore_anthill.c\
			pathfinding.c open_the_gates.c queue.c paths.c\
			visualizer_main.c init_visualizer.c loop.c visualize.c\
			render_anthill.c render_ants.c render_numbers.c

SRCS_DIR := src

OBJS := $(SRCSFILES:.c=.o)

OBJ_DIR := obj

LFT := libft.a

LFTPRINTF := libftprintf.a

LFT_DIR := src/libs/libft

LFTPRINTF_DIR := src/libs/ft_printf

HEADERS := lemin.h libft.h ft_printf.h SDL.h SDL_ttf.h

HEADERS_DIR := include $(LFT_DIR) $(LFTPRINTF_DIR)/includes src/libs/sdl2/include/SDL2 src/libs/sdl2_ttf/include/SDL2

SDL_LIBS := -L ~/.brew/lib -lSDL2 -lSDL2_ttf

CC := gcc -Wall -Werror -Wextra

vpath %.c $(SRCS_DIR)/solver $(SRCS_DIR)/visualizer
vpath %.o $(OBJ_DIR)
vpath %.h $(HEADERS_DIR)
vpath %.a $(LFT_DIR) $(LFTPRINTF_DIR)

all: $(NAME)

$(NAME): $(LFT) $(LFTPRINTF) $(OBJS)
	@$(CC) $(addprefix $(OBJ_DIR)/, $(OBJS)) $(INCLUDE) -lft -L $(LFT_DIR) -lftprintf -L $(LFTPRINTF_DIR) $(SDL_LIBS) -o $@
	@printf "\r\e[J\e[32m$@\e[0m done!\n\e[?25h"

$(OBJS): %.o: %.c $(HEADERS)
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c $< $(addprefix -I,$(HEADERS_DIR)) -o $(OBJ_DIR)/$@
	@printf "\r\e[?25l\e[Jcompiling \e[32m$(notdir $<)\e[0m"

$(LFT):
	@$(MAKE) -C $(LFT_DIR)
	@$(MAKE) -C $(LFT_DIR) clean

$(LFTPRINTF):
	@$(MAKE) -C $(LFTPRINTF_DIR)
	@$(MAKE) -C $(LFTPRINTF_DIR) clean

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LFT_DIR) fclean
	@$(MAKE) -C $(LFTPRINTF_DIR) fclean

fclean: clean
	@rm -f $(NAME)

re: fclean all
