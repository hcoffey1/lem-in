/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 17:14:49 by smorty            #+#    #+#             */
/*   Updated: 2019/08/30 22:44:18 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void	init_visualizer(SDL_Window **window, t_lemin_vis **game)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		error_vis(SDL_GetError());
	if (!(*window = SDL_CreateWindow("smorty's lem-in",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		LEMIN_SCREEN_WIDTH, LEMIN_SCREEN_HEIGHT, SDL_WINDOW_SHOWN)))
		error_vis(SDL_GetError());
	if (TTF_Init())
		error_vis(TTF_GetError());
	if (!(*game = (t_lemin_vis *)malloc(sizeof(t_lemin_vis))))
		error(errno);
	if (!((*game)->font = TTF_OpenFont("/srcs/OpenSans-Semibold.ttf", 72)))
		error_vis(TTF_GetError());
	if (!((*game)->renderer = SDL_CreateRenderer(*window, -1, 0)))
		error_vis(SDL_GetError());
	(*game)->quit = 0;
	(*game)->pause = 0;
	(*game)->ended = 0;
	(*game)->speed = 0;
	(*game)->print = 0;
	(*game)->controls = 0;
	(*game)->color = 0;
}

static void	scale_verteces_position(const t_lemin *colony, float *x_scale, float *y_scale)
{
	int i;
	int max_y;
	int max_x;

	max_y = -1;
	max_x = -1;
	i = colony->verteces;
	while (i--)
	{
		if (max_x < colony->rooms[i]->x)
			max_x = colony->rooms[i]->x;
		if (max_y < colony->rooms[i]->y)
			max_y = colony->rooms[i]->y;
	}
	*x_scale = (float)(LEMIN_SCREEN_WIDTH - 50) / (max_x + 2);
	*y_scale = (float)(LEMIN_SCREEN_HEIGHT - 50) / (max_y + 2);
}

static void	set_color(SDL_Color *color, int choice)
{
	ft_memset(color, 200, sizeof(color));
	if (choice == 1)
		color->r = 100;
	else if (choice == 2)
		color->g = 100;
	color->a = 0;
}

static void render_verteces(t_lemin_vis *game, const t_lemin *colony)
{
	SDL_Rect	vert;
	SDL_Color	color;
	int			i;
	float		x_scale;
	float		y_scale;

	scale_verteces_position(colony, &x_scale, &y_scale);
	i = colony->verteces;
	vert.w = LEMIN_SCREEN_WIDTH / 2 / i;
	vert.h = LEMIN_SCREEN_HEIGHT / 2 / i;
	if (!vert.w)
		vert.w = 1;
	if (vert.h < vert.w)
		vert.h = vert.w;
	else
		vert.w = vert.h;
	while (i--)
	{
		if (colony->rooms[i] == colony->start)
			set_color(&color, 1);
		else if (colony->rooms[i] == colony->end)
			set_color(&color, 2);
		else
			set_color(&color, 0);
		vert.x = 25 + (colony->rooms[i]->x + 1) * x_scale - vert.w / 2;
		vert.y = 25 + (colony->rooms[i]->y + 1) * y_scale - vert.h / 2;
		if (SDL_SetRenderDrawColor(game->renderer, color.r, color.g, color.b, 0)
		|| SDL_RenderFillRect(game->renderer, &vert))
			error_vis(SDL_GetError());
	}
}

static void	visualize(t_lemin_vis *game, const t_lemin *colony)
{
	if (SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0)
		|| SDL_RenderClear(game->renderer))
		error_vis(SDL_GetError());
	render_verteces(game, colony);
//	put_score(game);
//	put_controls(game, controls);
	SDL_RenderPresent(game->renderer);
}

static void	key_press(t_lemin_vis *game, SDL_Keycode key)
{
	if (key == SDLK_SPACE)
		game->pause = ~game->pause;
	else if (key == SDLK_KP_PLUS && game->speed)
		--game->speed;
	else if (key == SDLK_KP_MINUS && game->speed < 100)
		++game->speed;
	else if (key == SDLK_KP_ENTER)
		game->print = ~game->print;
	else if (key == SDLK_TAB)
		game->controls = ~game->controls;
	// else if (key == SDLK_LEFT && game->pause)
	// 	next_board(game, game, 0);
	// else if (key == SDLK_RIGHT && game->pause)
	// 	next_board(game, game, 1);
	// else if (key == SDLK_BACKSPACE)
	// 	change_colors(game, (game->color += (game->color < 8 ? 1 : -8)));
	else if (key == SDLK_ESCAPE)
		game->quit = 1;
}

void		visualization_loop(t_lemin_vis *game, const t_lemin *colony, t_input *map)
{
	SDL_Event		e;
	int				delay;

	(void)map;
	delay = 0;
	while (!game->quit)
	{
		visualize(game, colony);
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
				key_press(game, e.key.keysym.sym);
			if (e.type == SDL_WINDOWEVENT
				&& e.window.event == SDL_WINDOWEVENT_CLOSE)
				game->quit = 1;
		}
	}
}

void		visualizer(const t_lemin *colony, t_input *map)
{
	t_lemin_vis *game;
	SDL_Window	*window;

	(void)map;
	init_visualizer(&window, &game);
	visualization_loop(game, colony, map);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
