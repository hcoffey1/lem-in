/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 17:14:49 by smorty            #+#    #+#             */
/*   Updated: 2019/09/05 00:03:37 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void			create_ant_sprite(t_lemin_vis *game,
									t_ants_vis **list, t_ants *ant)
{
	t_ants_vis	*new;

	if (!(new = (t_ants_vis *)malloc(sizeof(t_ants_vis))))
		error(errno);
	if (!(new->ant = (SDL_Rect *)malloc(sizeof(SDL_Rect))))
		error(errno);
	new->ant->w = game->node_size / 2;
	new->ant->h = game->node_size / 2;
	new->curr_x = ant->room->screen_x - game->node_size / 4;
	new->curr_y = ant->room->screen_y - game->node_size / 4;
	new->x_dest = ant->path->next->top->screen_x - game->node_size / 4;
	new->y_dest = ant->path->next->top->screen_y - game->node_size / 4;
	new->delta_x = (new->x_dest - new->curr_x)
				/ hypot(new->x_dest - new->curr_x, new->y_dest - new->curr_y);
	new->delta_y = (new->y_dest - new->curr_y)
				/ hypot(new->x_dest - new->curr_x, new->y_dest - new->curr_y);
	new->next = NULL;
	new->prev = *list;
	if (*list)
		(*list)->next = new;
	*list = new;
}

static t_ants_vis	*next_move(t_lemin_vis *game, t_ants *ants)
{
	t_ants_vis *ants_list;

	ants_list = NULL;
	while (ants)
	{
		if (ants->room != game->colony->end && !ants->path->next->top->closed)
		{
			create_ant_sprite(game, &ants_list, ants);
			ants->room->closed = 0;
			ants->path = ants->path->next;
			ants->room = ants->path->top;
			if (ants->room != game->colony->end)
				ants->room->closed = 1;
		}
		ants = ants->next;
	}
	while (ants_list->prev)
		ants_list = ants_list->prev;
	++game->moves;
	return (ants_list);
}

static void			key_press(t_lemin_vis *game, SDL_Keycode key)
{
	if (key == SDLK_SPACE)
		game->status->pause = ~game->status->pause;
	else if ((key == SDLK_KP_MINUS || key == SDLK_MINUS)
		&& game->status->speed > 1)
		--game->status->speed;
	else if ((key == SDLK_KP_PLUS || key == SDLK_EQUALS)
		&& game->status->speed < 10)
		++game->status->speed;
	else if (key == SDLK_ESCAPE)
		game->status->quit = 1;
}

static int			color_path(int **edges, t_paths *solution, int speed)
{
	static int delay = 50;

	if (delay--)
		return (0);
	delay = 150 / speed;
	while (solution)
	{
		if (solution->path->next)
		{
			++edges[solution->path->top->index]
				[solution->path->next->top->index];
			solution->path = solution->path->next;
			return (0);
		}
		solution = solution->next;
	}
	return (1);
}

void				visualization_loop(t_lemin_vis *game, t_paths *solution)
{
	t_ants_vis	*ants_list;
	SDL_Event	e;

	ants_list = NULL;
	while (!game->status->quit)
	{
		visualize(game, &ants_list);
		if (!game->status->pause)
		{
			if (!game->status->paths)
				game->status->paths = color_path(game->colony->edges,
										solution, game->status->speed);
			else if (!game->status->ended && !ants_list)
				ants_list = next_move(game, game->colony->ants);
		}
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
				key_press(game, e.key.keysym.sym);
			if (e.type == SDL_WINDOWEVENT
				&& e.window.event == SDL_WINDOWEVENT_CLOSE)
				game->status->quit = 1;
		}
	}
}
