/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dijkstra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 18:59:18 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 17:30:01 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

t_queue		*get_path(t_lemin *colony)
{
	static int n = 0;
	t_vertex *p;
	t_queue	*path;

	ft_printf("{yellow}path %d\n{eoc}", ++n);
	p = colony->end;
	path = new_queue(p);
	while (p != colony->start)
	{
		ft_printf("%s ", p->name);
//		colony->edges[p->path->index][p->index]->open = 0;
  //      colony->edges[p->path->index][p->index]->weight *= -1;
    //    colony->edges[p->index][p->path->index]->weight *= -1;
		if (p != colony->end)
			split_vertex(colony, p);
		p = p->path;
		push_front(&path, p);
	}
	ft_printf("%s\n", p->name);
	return (path);
}

t_queue		*dijkstra_algorithm(t_lemin *all)
{
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
            if (all->edges[parent][child] && all->edges[parent][child]->open && !all->rooms[child]->visited)
            {
                if (all->edges[parent][child]->weight + all->rooms[parent]->minpath < all->rooms[child]->minpath)
				{
                    all->rooms[child]->minpath = all->edges[parent][child]->weight + all->rooms[parent]->minpath;
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

/*t_queue			*bfs(t_vertex *start)
{
	t_queue		*q;
	t_vertex	*p;

	q = new_queue(start);
	start->visited = 1;
	while (start->type != 4) // пока не нашлась комната с типом end
	{
		while (->edges[])
		{
			if (!p->link->visited && !p->link->closed) // добавляем все связанные комнаты в очередь, кроме уже просмотренных из других комнат (visited),
			{															//либо закрытых другими найденными путями (closed)
				push(&q, p->link);
				p->link->path = start; // отмечаем из какой комнаты пришли, чтобы потом составить маршрут
				p->link->visited = 1;
			}
			p = p->next;
		}
		pop(&q); // удаляем первый элемент из очереди (текущую комнату)
		if (!q)	// если комнат больше не осталось, значит, все уже проверены, и пути нет
			return (NULL);
		start = q->val;
	}
	while (q)
		pop(&q);
	return (get_path(start)); // составляем маршрут в виде очереди комнат
}*/
/*
t_queue            *bellman_ford_algorithm(t_lemin *ant_farm)
{
    int            vertex;
    int            parent;
    int            child;

    ant_farm->start->minpath = 0;
    vertex = 0;
    while (ant_farm->rooms[vertex])
    {
        parent = 0;
        while (parent < ant_farm->verteces)
        {
            child = 0;
            while (child < ant_farm->verteces)
            {
                if (ant_farm->edges[parent][child])
                {
                    if (ant_farm->edges[parent][child]->weight + ant_farm->rooms[vertex]->minpath < ant_farm->rooms[child]->minpath)
                        ant_farm->rooms[child]->minpath = ant_farm->edges[parent][child]->weight + ant_farm->rooms[vertex]->minpath;
                   	ant_farm->rooms[child]->path = ant_farm->rooms[parent];
               	}
                ++child;
            }
            ++parent;
        }
        ++vertex;
    }
    return (get_path(ant_farm));
}*/
/*
t_queue            *bellman_ford_algorithm(t_lemin *ant_farm)
{
    int            vertex;
    int            parent;
    int            child;

    ant_farm->start->minpath = 0;
    vertex = 0;
    while (vertex < ant_farm->verteces)
    {
        parent = ant_farm->rooms[vertex]->index;
        child = 0;
        while (child < ant_farm->verteces)
        {
            if (ant_farm->edges[parent][child])
            {
                if (ant_farm->edges[parent][child]->weight + ant_farm->rooms[parent]->minpath < ant_farm->rooms[child]->minpath)
                    ant_farm->rooms[child]->minpath = ant_farm->edges[parent][child]->weight + ant_farm->rooms[parent]->minpath;
               	ant_farm->rooms[child]->path = ant_farm->rooms[parent];
          	}
            ++child;
        }
        ++vertex;
    }
    return (get_path(ant_farm));
}*/
