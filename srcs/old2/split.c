/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 19:09:04 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 17:30:01 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	split_vertex(t_lemin *colony, t_vertex *room)
{
	t_vertex	*new;
	int			i;
	int			j;

	new = (t_vertex *)malloc(sizeof(t_vertex));
	new->name = room->name;
	new->index = colony->verteces - room->index;
	new->minpath = room->minpath;
	new->type = 2;
	new->visited = 0;
	new->x = 0;
	new->y = 0;
	colony->rooms[new->index] = new;
	i = room->index;
	j = 0;
	while (j < colony->verteces)
	{
		if (colony->edges[i][j])
		{
			if (colony->rooms[j] == colony->end)
				colony->edges[i][j] = NULL;
			else
			{
				colony->edges[j][new->index] = colony->edges[j][i];
				colony->edges[j][i] = NULL;
			}
		}
		++j;
	}
	colony->edges[i][new->index] = new_link(room, new);
}
