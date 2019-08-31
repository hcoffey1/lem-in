/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 17:14:49 by smorty            #+#    #+#             */
/*   Updated: 2019/08/31 19:06:34 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_assets *get_assets(SDL_Renderer *renderer)
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
	return (assets);
}

static void			init_visualizer(SDL_Window **window, t_lemin_vis **game)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		error_vis(SDL_GetError());
	if (!(*window = SDL_CreateWindow("smorty's lem-in",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		LEMIN_SCREEN_WIDTH, LEMIN_SCREEN_HEIGHT, SDL_WINDOW_SHOWN)))
		error_vis(SDL_GetError());
	if (!(*game = (t_lemin_vis *)malloc(sizeof(t_lemin_vis))))
		error(errno);
	if (!((*game)->renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE)))
		error_vis(SDL_GetError());
	(*game)->assets = get_assets((*game)->renderer);
	(*game)->quit = 0;
	(*game)->pause = 0;
	(*game)->ended = 0;
	(*game)->speed = 0;
	(*game)->print = 0;
	(*game)->controls = 0;
	(*game)->color = 0;
}

static void			scale_verteces_position(const t_lemin *colony, float *x_scale, float *y_scale)
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

// static void	set_color(SDL_Color *color, int choice)
// {
// 	ft_memset(color, 200, sizeof(color));
// 	if (choice == 1)
// 		color->r = 100;
// 	else if (choice == 2)
// 		color->g = 100;
// 	color->a = 0;
// }

void				draw_circle(SDL_Renderer *renderer, SDL_Point center, int radius, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++)
    {
        for (int h = 0; h < radius * 2; h++)
        {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx*dx + dy*dy) <= (radius * radius))
            {
                SDL_RenderDrawPoint(renderer, center.x + dx, center.y + dy);
            }
        }
    }
}

static void	put_name(t_lemin_vis *game, char *text)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	SDL_Color	color;
	SDL_Rect	rect;

	if (!(surface = TTF_RenderText_Blended(game->assets->font, text, color)))
		error(SDL_GetError());
	if (!(texture = SDL_CreateTextureFromSurface(game->renderer, surface)))
		error(SDL_GetError());
//	rect.w = ft_strlen(text) * (scale > 0 ? scale : -scale);
	if (SDL_RenderCopy(game->renderer, texture, NULL, &rect))
		error(SDL_GetError());
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
// 	if (scale > 0)
// 		rect->x += rect->w;
// 	else
// 		rect->y += rect->h;
// }
}

static void render_verteces(t_lemin_vis *game, const t_lemin *colony, float x_scale, float y_scale)
{
	SDL_Texture	*node;
	SDL_Rect	rect;
	int			i;

	i = colony->verteces;
	rect.w = LEMIN_SCREEN_WIDTH / 2 / i;
	rect.h = LEMIN_SCREEN_HEIGHT / 2 / i;
	if (rect.w > rect.h)
		rect.w = rect.h;
	else
		rect.h = rect.w;
	while (i--)
	{
		rect.x = (colony->rooms[i]->x + 1) * x_scale;
		rect.y = (colony->rooms[i]->y + 1) * y_scale;
		if (colony->rooms[i] == colony->start)
			node = game->assets->start_img;
		else if (colony->rooms[i] == colony->end)
			node = game->assets->end_img;
		else
			node = game->assets->node_img;
		SDL_RenderCopy(game->renderer, node, NULL, &rect);
	}
}

static void render_edges(t_lemin_vis *game, const t_lemin *colony,
							float x_scale, float y_scale)
{
	int i;
	int j;

	SDL_SetRenderDrawColor(game->renderer, 100, 150, 50, 0);
	i = colony->verteces;
	while (i--)
	{
		j = colony->verteces;
		while (j--)
			if (colony->edges[i][j])
				SDL_RenderDrawLine(game->renderer,
					(colony->rooms[i]->x + 1) * x_scale,
					(colony->rooms[i]->y + 1) * y_scale,
					(colony->rooms[j]->x + 1) * x_scale,
					(colony->rooms[j]->y + 1) * y_scale);
	}
}


static void	visualize(t_lemin_vis *game, const t_lemin *colony)
{
	float x_scale;
	float y_scale;

	if (SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0)
		|| SDL_RenderClear(game->renderer))
		error_vis(SDL_GetError());
	scale_verteces_position(colony, &x_scale, &y_scale);
	render_verteces(game, colony, x_scale, y_scale);
	render_edges(game, colony, x_scale, y_scale);
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
