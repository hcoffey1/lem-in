/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualizer_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/03 21:42:13 by smorty            #+#    #+#             */
/*   Updated: 2019/09/04 22:29:32 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin_visualizer.h"

static void	restore_edges(t_lemin *colony, int **edges)
{
	int		i;

	i = colony->verteces;
	while (i--)
	{
		free(colony->edges[i]);
		colony->edges[i] = edges[i];
	}
	free(edges);
	i = colony->verteces;
}

static void	return_ants_to_start(t_ants *ants, t_vertex *start)
{
	while (ants)
	{
		ants->room = start;
		while (ants->path->prev)
			ants->path = ants->path->prev;
		ants = ants->next;
	}
}

static void	cleanup(t_lemin_vis *game)
{
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyTexture(game->assets->node);
	SDL_DestroyTexture(game->assets->start);
	SDL_DestroyTexture(game->assets->end);
	SDL_DestroyTexture(game->assets->closed);
	SDL_DestroyTexture(game->assets->ant);
	TTF_CloseFont(game->assets->font);
	free(game->assets);
	free(game->status);
	free(game);
}

void		visualizer_main(t_lemin *colony, t_paths *solution)
{
	t_lemin_vis *game;
	SDL_Window	*window;

	restore_edges(colony, colony->edges_backup);
	return_ants_to_start(colony->ants, colony->start);
	game = init_visualizer(&window, colony);
	visualization_loop(game, solution);
	cleanup(game);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
