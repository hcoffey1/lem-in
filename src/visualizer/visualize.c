/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 21:58:25 by smorty            #+#    #+#             */
/*   Updated: 2019/09/04 22:44:55 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

SDL_Texture	*get_string_texture(t_lemin_vis *game, const char *text)
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

static void	render_controls(t_lemin_vis *game)
{
	SDL_Rect	rect;
	SDL_Texture	*texture;
	const char	*text;

	text = "Esc: quit, +/-: speed, spacebar: ";
	rect.x = SCREEN_OFFSET;
	rect.y = LEMIN_SCREEN_HEIGHT - SCREEN_OFFSET / 4 - rect.h;
	rect.h = 50;
	rect.w = ft_strlen(text) * 20;
	texture = get_string_texture(game, text);
	if (SDL_RenderCopy(game->renderer, texture, NULL, &rect))
		error_vis(SDL_GetError());
	SDL_DestroyTexture(texture);
	text = game->status->pause ? "continue" : "pause  ";
	rect.x += rect.w;
	rect.w = ft_strlen(text) * 20;
	texture = get_string_texture(game, text);
	if (SDL_RenderCopy(game->renderer, texture, NULL, &rect))
		error_vis(SDL_GetError());
	SDL_DestroyTexture(texture);
}

void		visualize(t_lemin_vis *game, t_ants_vis **ants_list)
{
	if (SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0)
		|| SDL_RenderClear(game->renderer))
		error_vis(SDL_GetError());
	render_edges(game);
	if (*ants_list)
		render_ants(game, ants_list);
	render_nodes(game);
	render_numbers(game);
	render_controls(game);
	SDL_RenderPresent(game->renderer);
}
