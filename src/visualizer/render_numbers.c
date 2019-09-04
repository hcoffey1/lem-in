/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_numbers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 20:13:17 by smorty            #+#    #+#             */
/*   Updated: 2019/09/04 23:35:54 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void	put_text(t_lemin_vis *game, SDL_Rect *rect, char *p, char *text)
{
	SDL_Texture	*texture;
	char		*s;

	s = ft_strjoin(p, text);
	rect->w = ft_strlen(s) * 20;
	rect->x = (*p == 'S' ? SCREEN_OFFSET :
		LEMIN_SCREEN_WIDTH - SCREEN_OFFSET - rect->w);
	texture = get_string_texture(game, s);
	if (SDL_RenderCopy(game->renderer, texture, NULL, rect))
		error_vis(SDL_GetError());
	SDL_DestroyTexture(texture);
	free(s);
}

static int	find_ants(const t_lemin *colony, int *ants_start, int *ants_end)
{
	t_ants *ants;

	*ants_start = 0;
	*ants_end = 0;
	ants = colony->ants;
	while (ants)
	{
		if (ants->room == colony->start)
			++(*ants_start);
		if (ants->room == colony->end)
			++(*ants_end);
		ants = ants->next;
	}
	if (*ants_end == colony->ants_num)
		return (1);
	return (0);
}

void		render_numbers(t_lemin_vis *game)
{
	SDL_Rect	rect;
	char		*s;
	int			ants_end;
	int			ants_start;

	game->status->ended = find_ants(game->colony, &ants_start, &ants_end);
	rect.h = SCREEN_OFFSET;
	rect.y = SCREEN_OFFSET / 4;
	s = ft_itoa(ants_start);
	put_text(game, &rect, "Start: ", s);
	free(s);
	s = ft_itoa(ants_end);
	put_text(game, &rect, "End: ", s);
	free(s);
	rect.y = LEMIN_SCREEN_HEIGHT - SCREEN_OFFSET / 4 - rect.h;
	s = ft_itoa(game->moves);
	put_text(game, &rect, "Moves: ", s);
	free(s);
}
