/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 20:51:55 by smorty            #+#    #+#             */
/*   Updated: 2019/07/31 00:25:25 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	clear_path(t_vertex **rooms, int verteces)
{
	while (verteces--)
		if (rooms[verteces])
		{
			rooms[verteces]->visited = 0;
			rooms[verteces]->minpath = INF_PATH;
			rooms[verteces]->path = NULL;
		}
}

t_queue	*get_path(t_lemin *colony)
{
	t_vertex	*track;
	t_queue		*path;

	path = new_queue(colony->end);
	track = colony->end->path;
	while (track)
	{
		track->splitted = 1;
		colony->edges[track->index][path->top->index] = 0;
		if (colony->edges[path->top->index][track->index] > 0)
			colony->edges[path->top->index][track->index] *= -1;
		push_front(&path, track);
		track = track->path;
	}
	path->top->splitted = 0;
	clear_path(colony->rooms, colony->verteces);
	return (path);
}

t_queue	*dijkstra(t_lemin *colony)
{
	t_queue	*q;
	t_queue	*p;
	int		parent;
	int		child;
	int		distance;

	q = new_queue(colony->start);
	colony->start->minpath = 0;
	colony->start->visited = 1;
	while (q->top != colony->end)
	{
		parent = q->top->index;
		child = 0;
		while (child < colony->verteces)
		{
			if (colony->edges[parent][child] && !colony->rooms[child]->visited)
			{
				if (!colony->rooms[parent]->splitted || (colony->rooms[parent]->splitted && colony->rooms[parent]->path && colony->rooms[parent]->path->splitted)
					|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path && !colony->rooms[parent]->path->splitted && colony->edges[parent][child] < 0))
				{
					colony->rooms[child]->path = colony->rooms[parent];
/*					distance = ABS(colony->edges[parent][child]) + colony->rooms[parent]->minpath;
					if (colony->rooms[child]->minpath > distance)
					{
						colony->rooms[child]->minpath = distance;
					}*/
						push(&q, colony->rooms[child]);
						q->top->visited = 1;
				}
			}
			++child;
		}
		p = q;
		while (p->prev)
			p = p->prev;
		while (p)
		{
			ft_printf("%s ", p->top->name);
			p = p->next;
		}
		ft_printf("\n");
		pop(&q);
		if (!q)
			return (NULL);
	}
	while (q)
		pop(&q);
	return (get_path(colony));
}
