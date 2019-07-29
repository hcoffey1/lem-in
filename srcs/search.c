/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 20:51:55 by smorty            #+#    #+#             */
/*   Updated: 2019/07/29 23:58:30 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	split_vertex(t_lemin *colony, t_vertex *room)
{
	t_vertex	*new;
	int			i;

	new = (t_vertex *)malloc(sizeof(t_vertex));
	new->name = room->name;
	new->index = colony->verteces - room->index;
	new->type = 2;
	new->path = NULL;
	new->minpath = INF_PATH;
	new->visited = 0;
	new->x = 0;
	new->y = 0;
	colony->rooms[new->index] = new;
	i = 0;
	while (i < colony->verteces)
	{
		if (colony->adjacency[room->index][i])
		{
			if (room->path != colony->rooms[i])
			{
				colony->adjacency[i][new->index] = colony->adjacency[i][room->index];
				colony->adjacency[i][room->index] = 0;
			}
			else
			{
				colony->adjacency[new->index][i] = colony->adjacency[room->index][i] * -1;
				colony->adjacency[room->index][i] = 0;
				colony->adjacency[i][room->index] = 0;
			}
		}
		++i;
	}
	colony->adjacency[room->index][new->index] = ZERO_WEIGHT;
}

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

t_queue	*get_path(t_lemin *colony, int flag)
{
	t_vertex	*track;
	t_queue		*path;

	path = new_queue(colony->end);
	track = colony->end->path;
	while (track->path)
	{
		push_front(&path, track);
		if (flag)
		{
			if (colony->adjacency[path->top->index][track->index] != ZERO_WEIGHT)
				colony->adjacency[path->top->index][track->index] = colony->adjacency[path->top->index][track->index] * -1;
			colony->adjacency[track->index][path->top->index] = 0;
			split_vertex(colony, track);
		}
		track = track->path;
	}
	push_front(&path, track);
	clear_path(colony->rooms, colony->verteces);
	return (path);
}

t_queue	*dijkstra(t_lemin *colony, int flag)
{
	t_queue	*q;
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
			if (colony->adjacency[parent][child] && !colony->rooms[child]->visited)
			{
				distance = colony->adjacency[parent][child] == ZERO_WEIGHT ? 0 : colony->adjacency[parent][child];
				distance += q->top->minpath;
				if (colony->rooms[child]->minpath > distance)
				{
					colony->rooms[child]->minpath = distance;
					colony->rooms[child]->path = colony->rooms[parent];
				}
				push(&q, colony->rooms[child]);
				q->top->visited = 1;
			}
			++child;
		}
		pop(&q);
		if (!q)
			return (NULL);
	}
	while (q)
		pop(&q);
	return (get_path(colony, flag));
}
