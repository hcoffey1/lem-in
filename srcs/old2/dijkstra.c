/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dijkstra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 18:59:18 by smorty            #+#    #+#             */
/*   Updated: 2019/08/06 18:35:04 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_queue	*get_path_dijkstra(t_lemin *colony, int *len)
{
	t_vertex	*track;
	t_queue		*path;

	path = new_queue(colony->end);
	track = colony->end->path;
	*len = 0;
	while (track)
	{
		track->splitted = 1;
		colony->edges[track->index][path->top->index] = 0;
		colony->edges[path->top->index][track->index] *= -1;
		push_front(&path, track);
		track = track->path;
		++(*len);
	}
	colony->start->splitted = 0;
	clean_after_search(colony->rooms, colony->verteces);
	return (path);
}

static void		relax_and_push(t_lemin *colony, t_queue *q, int parent, int child)
{
	int weight;

	if (!colony->rooms[parent]->splitted
		|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path
			&& (colony->rooms[parent]->path->splitted
				|| colony->edges[parent][child] < 0)))
	{
		weight = colony->edges[parent][child] + colony->rooms[parent]->minpath;
		if (weight < colony->rooms[child]->minpath)
		{
			colony->rooms[child]->minpath = weight;
			colony->rooms[child]->path = colony->rooms[parent];
		}
		push(&q, colony->rooms[child]);
	}
}

t_queue			*dijkstra(t_lemin *colony, int *len)
{
	t_queue	*q;
	int		parent;
	int		child;

	q = new_queue(colony->start);
	colony->start->minpath = 0;
	while (q->top != colony->end)
	{
		if (!q->top->closed)
		{
			q->top->closed = 1;
			parent = q->top->index;
			child = -1;
			while (++child < colony->verteces)
				if (colony->edges[parent][child] && !colony->rooms[child]->closed)
					relax_and_push(colony, q, parent, child);
		}
		pop(&q);
		if (!q)
			return (NULL);
	}
	while (q)
		pop(&q);
	return (get_path_dijkstra(colony, len));
}
