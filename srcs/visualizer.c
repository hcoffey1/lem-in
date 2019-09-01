/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 17:14:49 by smorty            #+#    #+#             */
/*   Updated: 2019/09/01 23:34:22 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static SDL_Texture	*get_name_texture(t_lemin_vis *game, char *text)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	SDL_Color	color;

	ft_memset(&color, 255, sizeof(color));
	color.a = 0;
	if (!(surface = TTF_RenderText_Blended(game->assets->font, text, color)))
		error_vis(SDL_GetError());
	if (!(texture = SDL_CreateTextureFromSurface(game->renderer, surface)))
		error_vis(SDL_GetError());
	SDL_FreeSurface(surface);
	return (texture);
}

static void	draw_connection(t_lemin_vis *game, int i, SDL_Rect node)
{
	int x;
	int y;
	int j;

	node.x += node.w / 2;
	node.y += node.h / 2;
	j = game->colony->verteces;
	while (j--)
	{
		if (game->colony->edges[i][j])
		{
			if (game->colony->edges[i][j] == game->colony->edges[j][i])
				SDL_SetRenderDrawColor(game->renderer, 100, 150, 50, 0);
			else
				SDL_SetRenderDrawColor(game->renderer, 50, 50, 200, 0);
			x = (game->colony->rooms[j]->x + 1) * game->x_scale + node.w / 2;
			y = (game->colony->rooms[j]->y + 1) * game->y_scale + node.h / 2;
			SDL_RenderDrawLine(game->renderer, node.x, node.y, x, y);
		}
	}
}

static void render_anthill(t_lemin_vis *game)
{
	SDL_Texture	*node;
	SDL_Texture	*name;
	SDL_Rect	node_rect;
	SDL_Rect	name_rect;
	int			i;

	i = game->colony->verteces;
	node_rect.w = LEMIN_SCREEN_WIDTH / 2 / i;
	node_rect.h = LEMIN_SCREEN_HEIGHT / 2 / i;
	if (node_rect.w > node_rect.h)
		node_rect.w = node_rect.h;
	else
		node_rect.h = node_rect.w;
	name_rect.w = node_rect.w / 2;
	name_rect.h = LEMIN_SCREEN_HEIGHT / 4 / i;
	while (i--)
	{
		node_rect.x = (game->colony->rooms[i]->x + 1) * game->x_scale;
		node_rect.y = (game->colony->rooms[i]->y + 1) * game->y_scale;
		name_rect.x = node_rect.x + name_rect.w / 2;
		name_rect.y = node_rect.y + name_rect.h / 2;
		if (game->colony->rooms[i] == game->colony->start)
			node = game->assets->start_img;
		else if (game->colony->rooms[i] == game->colony->end)
			node = game->assets->end_img;
		else if (game->colony->rooms[i]->closed)
			node = game->assets->end_img;
		else
			node = game->assets->node_img;
		draw_connection(game, i, node_rect);
		SDL_RenderCopy(game->renderer, node, NULL, &node_rect);
		name = get_name_texture(game, game->colony->rooms[i]->name);
		SDL_RenderCopy(game->renderer, name, NULL, &name_rect);
		SDL_DestroyTexture(name);
	}
}

static int	calculate_ants_in_room(t_ants *ants, t_vertex *room)
{
	int count;

	count = 0;
	while (ants)
	{
		if (ants->room == room)
			++count;
		ants = ants->next;
	}
	return (count);
}

static int render_ants_count(t_lemin_vis *game)
{
	SDL_Rect	rect;
	SDL_Texture	*count;
	char		*text;
	int			finished;

	finished = calculate_ants_in_room(game->colony->ants, game->colony->end);
	rect.y = 15;
	rect.h = 50;
	if (!(text = ft_itoa(game->colony->ants_num - finished)))
		error(errno);
	rect.x = 25;
	rect.w = ft_strlen(text) * 30;
	count = get_name_texture(game, text);
	if (SDL_RenderCopy(game->renderer, count, NULL, &rect))
		error_vis(SDL_GetError());
	SDL_DestroyTexture(count);
	free(text);
	if (!(text = ft_itoa(finished)))
		error(errno);
	rect.x = LEMIN_SCREEN_WIDTH - 25 - ft_strlen(text) * 30;
	rect.w = ft_strlen(text) * 30;
	count = get_name_texture(game, text);
	if (SDL_RenderCopy(game->renderer, count, NULL, &rect))
		error_vis(SDL_GetError());
	SDL_DestroyTexture(count);
	free(text);
	return (finished);
}

static int render_ants(t_lemin_vis *game, t_ants_vis *ants_list)
{
//	float		delta_x;
//	float		delta_y;
	int count;

	count = 0;
	while (ants_list)
	{
		if (ants_list->ant->x != ants_list->x_dest || ants_list->ant->y != ants_list->y_dest)
		{
			SDL_RenderCopy(game->renderer, game->assets->ant_img, NULL, ants_list->ant);
			if (ants_list->ant->x != ants_list->x_dest)
				ants_list->ant->x += (ants_list->ant->x < ants_list->x_dest ? 1 : -1);
			if (ants_list->ant->y != ants_list->y_dest)
				ants_list->ant->y += (ants_list->ant->y < ants_list->y_dest ? 1 : -1);
			++count;
		}
		ants_list = ants_list->next;
	}
	return (count);
}

static t_ants_vis *destroy_ants_list(t_ants_vis *ants_list)
{
	while (ants_list->next)
	{
		ants_list = ants_list->next;
		free(ants_list->prev->ant);
		free(ants_list->prev);
	}
	free(ants_list->ant);
	free(ants_list);
	return (NULL);
}


static void	visualize(t_lemin_vis *game, t_ants_vis **ants_list)
{
	if (SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0)
		|| SDL_RenderClear(game->renderer))
		error_vis(SDL_GetError());
	render_anthill(game);
	if (*ants_list)
		if (!render_ants(game, *ants_list))
			*ants_list = destroy_ants_list(*ants_list);
	if (render_ants_count(game) == game->colony->ants_num)
		game->ended = 1;
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

static void	create_ant_object(t_lemin_vis *game, t_ants_vis **list, t_ants *ant)
{
	t_ants_vis	*new;

	if (!(new = (t_ants_vis *)malloc(sizeof(t_ants_vis))))
		error(errno);
	if (!(new->ant = (SDL_Rect *)malloc(sizeof(SDL_Rect))))
		error(errno);
	new->ant->x = (ant->room->x + 1) * game->x_scale;
	new->ant->y = (ant->room->y + 1) * game->y_scale;
	new->ant->w = 25;
	new->ant->h = 25;
	new->x_dest = (ant->path->next->top->x + 1) * game->x_scale;
	new->y_dest = (ant->path->next->top->y + 1) * game->y_scale;
	new->next = NULL;
	new->prev = *list;
	if (*list)
		(*list)->next = new;
	*list = new;
}

static t_ants_vis	*next_move(t_lemin_vis *game, t_ants *ants, t_vertex *end)
{
	t_ants_vis *ants_list;

	ants_list = NULL;
	while (ants)
	{
		if (ants->room != end && !ants->path->next->top->closed)
		{
			create_ant_object(game, &ants_list, ants);
			ants->room->closed = 0;
			ants->path = ants->path->next;
			ants->room = ants->path->top;
			if (ants->room != end)
				ants->room->closed = 1;
		}
		ants = ants->next;
	}
	while (ants_list->prev)
		ants_list = ants_list->prev;
	return (ants_list);
}

void		visualization_loop(t_lemin_vis *game)
{
	t_ants_vis *ants_list;
	SDL_Event	e;
	int			delay;

	ants_list = NULL;
	delay = 10;
	while (!game->quit)
	{
		visualize(game, &ants_list);
		if (!delay && !game->pause && !game->ended && !ants_list)
		{
			ants_list = next_move(game, game->colony->ants, game->colony->end);
			delay = 10;
		}
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_KEYDOWN)
				key_press(game, e.key.keysym.sym);
			if (e.type == SDL_WINDOWEVENT
				&& e.window.event == SDL_WINDOWEVENT_CLOSE)
				game->quit = 1;
		}
		if (delay)
			--delay;
	}
}

void		visualizer(t_lemin *colony, int **edges_copy, t_paths *solution)
{
	t_lemin_vis *game;
	SDL_Window	*window;

	prepare_visualization(colony, edges_copy, solution);
	game = init_visualizer(&window, colony);
	visualization_loop(game);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
