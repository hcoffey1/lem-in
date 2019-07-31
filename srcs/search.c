/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 20:51:55 by smorty            #+#    #+#             */
/*   Updated: 2019/07/31 22:51:11 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	clear_path(t_vertex **rooms, int verteces)
{
	while (verteces--)
		if (rooms[verteces])
		{
			rooms[verteces]->closed = 0;
			rooms[verteces]->minpath = INF_PATH;
			rooms[verteces]->path = NULL;
		}
}

t_queue	*get_path(t_lemin *colony, int *len)
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
	clear_path(colony->rooms, colony->verteces);
	return (path);
}

void		print_queue(t_queue *q)
{
	while (q->prev)
		q = q->prev;
	while (q)
	{
		ft_printf("%s ", q->top->name);
		q = q->next;
	}
	ft_printf("\n");
}

int        relax(int **edges, t_vertex **rooms, int parent, int child)
{
    if (edges[parent][child] + rooms[parent]->minpath < rooms[child]->minpath)
	{
        rooms[child]->minpath = edges[parent][child] + rooms[parent]->minpath;
		rooms[child]->path = rooms[parent];
		return (1);
	}
	return (0);
}

t_queue    *dijkstra(t_lemin *colony, int *len)
{
    t_queue    *q;
    t_queue    *p;
    int        parent;
    int        child;
    int        distance;

    q = new_queue(colony->start);
    colony->start->minpath = 0;
	colony->start->closed = 1;
    while (q->top != colony->end)
    {
        parent = q->top->index;
        child = 0;
        while (child < colony->verteces)
        {
            if (colony->edges[parent][child] && !colony->rooms[child]->closed)
			{
				if (!colony->rooms[parent]->splitted
				|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path && colony->rooms[parent]->path->splitted)
				|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path && !colony->rooms[parent]->path->splitted && colony->edges[parent][child] < 0))
				{
					relax(colony->edges, colony->rooms, parent, child);
					push(&q, colony->rooms[child]);
				}
				colony->rooms[child]->closed = 1;
			}
			++child;
        }
//		print_queue(q);
        pop(&q);
        if (!q)
            return (NULL);
    }
    while (q)
        pop(&q);
    return (get_path(colony, len));
}
