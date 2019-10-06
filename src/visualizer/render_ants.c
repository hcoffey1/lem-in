/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ants.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 22:34:22 by smorty            #+#    #+#             */
/*   Updated: 2019/09/03 22:40:41 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void	move_ant(t_ants_vis *ant, int speed)
{
	while (speed--)
	{
		if ((int)ant->curr_x != ant->x_dest)
			ant->curr_x += ant->delta_x;
		if ((int)ant->curr_y != ant->y_dest)
			ant->curr_y += ant->delta_y;
	}
}

static int	put_ants(t_lemin_vis *game, t_ants_vis *ants_list)
{
	int moved;

	moved = 0;
	while (ants_list)
	{
		if ((int)ants_list->curr_x != ants_list->x_dest
			|| (int)ants_list->curr_y != ants_list->y_dest)
		{
			ants_list->ant->x = round(ants_list->curr_x);
			ants_list->ant->y = round(ants_list->curr_y);
			SDL_RenderCopy(game->renderer, game->assets->ant,
							NULL, ants_list->ant);
			if (!game->status->pause)
				move_ant(ants_list, game->status->speed);
			++moved;
		}
		ants_list = ants_list->next;
	}
	return (moved);
}

void		render_ants(t_lemin_vis *game, t_ants_vis **ants_list)
{
	if (!put_ants(game, *ants_list))
	{
		while ((*ants_list)->next)
		{
			(*ants_list) = (*ants_list)->next;
			free((*ants_list)->prev->ant);
			free((*ants_list)->prev);
		}
		free((*ants_list)->ant);
		free((*ants_list));
		*ants_list = NULL;
	}
}
