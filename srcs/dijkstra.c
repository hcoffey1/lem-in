/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dijkstra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 18:59:18 by smorty            #+#    #+#             */
/*   Updated: 2019/07/28 00:04:30 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_queue		*get_path(t_lemin *colony)
{
	static int n = 0;
	t_vertex *p;
	t_vertex *clean;
	t_queue	*path;

	ft_printf("{yellow}path %d\n{eoc}", ++n);
	p = colony->end;
	path = new_queue(p);
	while (p != colony->start)
	{
		ft_printf("%s ", p->name);
		colony->adjacency[p->path->index][p->index]->open = 0;
        colony->adjacency[p->path->index][p->index]->weight *= -1;
        colony->adjacency[p->index][p->path->index]->weight *= -1;
		clean = p;
		p = p->path;
		clean->path = NULL;
		push_front(&path, p);
	}
	ft_printf("%s\n", p->name);
	return (path);
}

t_queue		*dijkstra_algorithm(t_lemin *all)
{
    t_vertex    *list_rooms;
    t_queue        *queue;
    int            parent;
    int            child;

    queue = new_queue(all->start);
    all->start->minpath = 0;
    while (queue->top != all->end)
    {
        child = 0;
        parent = queue->top->index;
        while (child < all->verteces)
        {
            if (all->adjacency[parent][child] && all->adjacency[parent][child]->open && !all->rooms[child]->visited)
            {
                if (all->adjacency[parent][child]->weight + all->rooms[parent]->minpath < all->rooms[child]->minpath)
				{
                    all->rooms[child]->minpath = all->adjacency[parent][child]->weight + all->rooms[parent]->minpath;
					all->rooms[child]->path = all->rooms[parent];
				}
				all->rooms[child]->visited = 1;
                push(&queue, all->rooms[child]);
            }
            ++child;
        }
        queue->top->visited = 1;
        pop(&queue);
		if (!queue)
			return (NULL);
    }
	return (get_path(all));
}
