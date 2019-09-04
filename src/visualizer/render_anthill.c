/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_anthill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 22:25:09 by smorty            #+#    #+#             */
/*   Updated: 2019/09/04 23:01:24 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void			put_line(t_lemin_vis *game, int i, int j)
{
	SDL_Rect	rect;
	SDL_Point	center;
	SDL_Texture	*line;
	float		delta_x;
	float		delta_y;

	delta_x = game->colony->rooms[j]->screen_x
			- game->colony->rooms[i]->screen_x;
	delta_y = game->colony->rooms[j]->screen_y
			- game->colony->rooms[i]->screen_y;
	rect.h = game->node_size / 4;
	rect.w = hypot(delta_x, delta_y);
	rect.x = game->colony->rooms[i]->screen_x - rect.h / 2;
	rect.y = game->colony->rooms[i]->screen_y - rect.h / 2;
	center.x = rect.h / 2;
	center.y = rect.h / 2;
	line = game->colony->edges[i][j] == game->colony->edges[j][i] ?
			game->assets->pipe : game->assets->path;
	if (SDL_RenderCopyEx(game->renderer, line, NULL, &rect,
					180 / M_PI * atan2(delta_y, delta_x),
					&center, SDL_FLIP_NONE))
		error_vis(SDL_GetError());
}

void				render_edges(t_lemin_vis *game)
{
	int i;
	int j;

	i = game->colony->verteces;
	while (i--)
	{
		j = i;
		while (j--)
			if (game->colony->edges[i][j])
				put_line(game, i, j);
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
		node_rect.x = game->colony->rooms[i]->screen_x - game->node_size / 2;
		node_rect.y = game->colony->rooms[i]->screen_y - game->node_size / 2;
		name_rect.x = node_rect.x + node_rect.w / 2 - name_rect.w / 2;
		name_rect.y = node_rect.y + node_rect.h / 2 - name_rect.h / 2;
		if (SDL_RenderCopy(game->renderer,
					pick_asset(game, game->colony->rooms[i]), NULL, &node_rect))
			error_vis(SDL_GetError());
		name = get_string_texture(game, game->colony->rooms[i]->name);
		if (SDL_RenderCopy(game->renderer, name, NULL, &name_rect))
			error_vis(SDL_GetError());
		SDL_DestroyTexture(name);
	}
}
