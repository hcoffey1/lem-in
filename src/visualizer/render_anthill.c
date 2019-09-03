/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_anthill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 22:25:09 by smorty            #+#    #+#             */
/*   Updated: 2019/09/03 23:29:22 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void			draw_line(t_lemin_vis *game, int i, int j)
{
	int x0;
	int y0;
	int x1;
	int y1;

	if (game->colony->edges[i][j] == game->colony->edges[j][i])
		SDL_SetRenderDrawColor(game->renderer, 200, 200, 200, 0);
	else
		SDL_SetRenderDrawColor(game->renderer, 50, 50, 200, 0);
	x0 = X_OFFSET + game->colony->rooms[i]->x * game->x_scale
		+ game->node_size / 2;
	y0 = Y_OFFSET + game->colony->rooms[i]->y * game->y_scale
		+ game->node_size / 2;
	x1 = X_OFFSET + game->colony->rooms[j]->x * game->x_scale
		+ game->node_size / 2;
	y1 = Y_OFFSET + game->colony->rooms[j]->y * game->y_scale
		+ game->node_size / 2;
	SDL_RenderDrawLine(game->renderer, x0, y0, x1, y1);
}

void				render_edges(t_lemin_vis *game)
{
	int i;
	int j;

	i = game->colony->verteces;
	while (i--)
	{
		j = game->colony->verteces;
		while (j--)
			if (game->colony->edges[i][j])
				draw_line(game, i, j);
	}
}

static SDL_Texture	*pick_asset(t_lemin_vis *game, t_vertex *room)
{
	if (room == game->colony->start)
		return (game->assets->start);
	if (room == game->colony->end)
		return (game->assets->end);
	if (room->closed)
		return (game->assets->closed);
	return (game->assets->node);
}

void				render_nodes(t_lemin_vis *game)
{
	SDL_Texture	*name;
	SDL_Rect	node_rect;
	SDL_Rect	name_rect;
	int			i;

	node_rect.w = game->node_size;
	node_rect.h = game->node_size;
	name_rect.w = node_rect.w / 4;
	name_rect.h = node_rect.h / 2;
	i = game->colony->verteces;
	while (i--)
	{
		node_rect.x = X_OFFSET + game->colony->rooms[i]->x * game->x_scale;
		node_rect.y = Y_OFFSET + game->colony->rooms[i]->y * game->y_scale;
		name_rect.x = node_rect.x + node_rect.w / 2 - name_rect.w / 2;
		name_rect.y = node_rect.y + node_rect.h / 2 - name_rect.h / 2;
		SDL_RenderCopy(game->renderer,
						pick_asset(game, game->colony->rooms[i]),
						NULL, &node_rect);
		name = get_string_texture(game, game->colony->rooms[i]->name);
		SDL_RenderCopy(game->renderer, name, NULL, &name_rect);
		SDL_DestroyTexture(name);
	}
}
