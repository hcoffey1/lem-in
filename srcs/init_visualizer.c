/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_visualizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/01 20:44:05 by smorty            #+#    #+#             */
/*   Updated: 2019/09/01 22:16:58 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void			prepare_visualization(t_lemin *colony, int **matrix, t_paths *solution)
{
	t_queue	*p;
	t_ants	*ants;
	int		i;

	i = colony->verteces;
	while (i--)
	{
		free(colony->edges[i]);
		colony->edges[i] = matrix[i];
	}
	free(matrix);
	i = colony->verteces;
	while (solution)
	{
		p = solution->path;
		while (p->next)
		{
			++colony->edges[p->top->index][p->next->top->index];
			p = p->next;
		}
		solution = solution->next;
	}
	ants = colony->ants;
	while (ants)
	{
		ants->room = colony->start;
		while (ants->path->prev)
			ants->path = ants->path->prev;
		ants = ants->next;
	}
}

static void		scale_verteces_position(t_lemin_vis *game)
{
	int i;
	int max_y;
	int max_x;

	max_y = -1;
	max_x = -1;
	i = game->colony->verteces;
	while (i--)
	{
		if (max_x < game->colony->rooms[i]->x)
			max_x = game->colony->rooms[i]->x;
		if (max_y < game->colony->rooms[i]->y)
			max_y = game->colony->rooms[i]->y;
	}
	game->x_scale = (float)(LEMIN_SCREEN_WIDTH - 50) / (max_x + 2);
	game->y_scale = (float)(LEMIN_SCREEN_HEIGHT - 50) / (max_y + 2);
}

static t_assets	*get_assets(SDL_Renderer *renderer)
{
	t_assets	*assets;
	SDL_Surface *surf;
	
	if (!(assets = (t_assets *)malloc(sizeof(t_assets))))
		error(errno);
	if (TTF_Init())
		error_vis(TTF_GetError());
	if (!(assets->font = TTF_OpenFont("/srcs/OpenSans-Semibold.ttf", 72)))
		error_vis(TTF_GetError());
	if (!(surf = SDL_LoadBMP("srcs/assets/node.bmp")))
		error_vis(SDL_GetError());
	if (!(assets->node_img = SDL_CreateTextureFromSurface(renderer, surf)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surf);
	if (!(surf = SDL_LoadBMP("srcs/assets/start.bmp")))
		error_vis(SDL_GetError());
	if (!(assets->start_img = SDL_CreateTextureFromSurface(renderer, surf)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surf);
	if (!(surf = SDL_LoadBMP("srcs/assets/end.bmp")))
		error_vis(SDL_GetError());
	if (!(assets->end_img = SDL_CreateTextureFromSurface(renderer, surf)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surf);
	if (!(surf = SDL_LoadBMP("srcs/assets/start.bmp")))
		error_vis(SDL_GetError());
	if (!(assets->ant_img = SDL_CreateTextureFromSurface(renderer, surf)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surf);
	return (assets);
}

t_lemin_vis		*init_visualizer(SDL_Window **window, const t_lemin *colony)
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
	if (!(game->renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE)))
		error_vis(SDL_GetError());
	game->assets = get_assets(game->renderer);
	game->colony = colony;
	scale_verteces_position(game);
	game->quit = 0;
	game->pause = 0;
	game->ended = 0;
	game->speed = 0;
	game->print = 0;
	game->controls = 0;
	game->color = 0;
	return (game);
}
