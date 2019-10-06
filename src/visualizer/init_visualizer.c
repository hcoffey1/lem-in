/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_visualizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 20:44:05 by smorty            #+#    #+#             */
/*   Updated: 2019/09/06 15:49:38 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void			get_rooms_real_coords(t_lemin_vis *game)
{
	int		i;
	float	k;

	i = game->colony->verteces;
	while (i--)
	{
		k = (float)LEMIN_SCREEN_WIDTH / 2;
		k = (k - game->colony->rooms[i]->x * game->x_scale) / k;
		game->colony->rooms[i]->screen_x = round(
			game->colony->rooms[i]->x * game->x_scale + SCREEN_OFFSET
			+ (SCREEN_OFFSET + game->node_size / 2) * k);
		k = (float)LEMIN_SCREEN_HEIGHT / 2;
		k = (k - game->colony->rooms[i]->y * game->y_scale) / k;
		game->colony->rooms[i]->screen_y = round(
			game->colony->rooms[i]->y * game->y_scale + SCREEN_OFFSET
			+ (SCREEN_OFFSET + game->node_size / 2) * k);
	}
}

static void			scale_verteces(t_lemin_vis *game)
{
	int i;
	int max_y;
	int max_x;

	i = game->colony->verteces;
	max_x = (LEMIN_SCREEN_WIDTH - SCREEN_OFFSET * 2) / i;
	max_y = (LEMIN_SCREEN_HEIGHT - SCREEN_OFFSET * 4) / i;
	if ((game->node_size = max_x < max_y ? max_x : max_y) < 4)
		error_vis("Too many nodes for proper visualization");
	max_x = 1;
	max_y = 1;
	while (i--)
	{
		if (max_x < game->colony->rooms[i]->x)
			max_x = game->colony->rooms[i]->x;
		if (max_y < game->colony->rooms[i]->y)
			max_y = game->colony->rooms[i]->y;
	}
	game->x_scale = (float)(LEMIN_SCREEN_WIDTH - SCREEN_OFFSET * 2) / max_x;
	game->y_scale = (float)(LEMIN_SCREEN_HEIGHT - SCREEN_OFFSET * 2) / max_y;
	get_rooms_real_coords(game);
}

static SDL_Texture	*get_file_texture(SDL_Renderer *renderer, const char *file)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;

	if (!(surface = SDL_LoadBMP(file)))
		error_vis(SDL_GetError());
	if (!(texture = SDL_CreateTextureFromSurface(renderer, surface)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surface);
	return (texture);
}

static t_assets		*get_assets(SDL_Renderer *renderer)
{
	t_assets *assets;

	if (!(assets = (t_assets *)malloc(sizeof(t_assets))))
		error(errno);
	assets->node = get_file_texture(renderer, "src/assets/node.bmp");
	assets->start = get_file_texture(renderer, "src/assets/start.bmp");
	assets->end = get_file_texture(renderer, "src/assets/end.bmp");
	assets->closed = get_file_texture(renderer, "src/assets/closed.bmp");
	assets->ant = get_file_texture(renderer, "src/assets/ant.bmp");
	assets->pipe = get_file_texture(renderer, "src/assets/pipe.bmp");
	assets->path = get_file_texture(renderer, "src/assets/path.bmp");
	if (TTF_Init())
		error_vis(TTF_GetError());
	if (!(assets->font = TTF_OpenFont("src/assets/OpenSans-Semibold.ttf", 72)))
		error_vis(TTF_GetError());
	return (assets);
}

t_lemin_vis			*init_visualizer(SDL_Window **window, const t_lemin *colony)
{
	t_lemin_vis *game;

	if (SDL_Init(SDL_INIT_EVERYTHING))
		error_vis(SDL_GetError());
	if (!(*window = SDL_CreateWindow("smorty's lem-in",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		LEMIN_SCREEN_WIDTH, LEMIN_SCREEN_HEIGHT, SDL_WINDOW_SHOWN)))
		error_vis(SDL_GetError());
	if (!(game = (t_lemin_vis *)malloc(sizeof(t_lemin_vis))))
		error(errno);
	if (!(game->renderer = SDL_CreateRenderer(*window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		error_vis(SDL_GetError());
	game->assets = get_assets(game->renderer);
	game->colony = colony;
	scale_verteces(game);
	game->moves = 0;
	if (!(game->status = (t_vis_status *)malloc(sizeof(t_vis_status))))
		error(errno);
	game->status->speed = 3;
	game->status->paths = 0;
	game->status->pause = 0;
	game->status->ended = 1;
	game->status->quit = 0;
	return (game);
}
