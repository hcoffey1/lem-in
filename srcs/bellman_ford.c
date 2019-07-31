
#include "lemin.h"
/*
t_queue		*bellman_ford_algorithm(t_lemin *ant_farm)
{
    int            vertex;
    int            parent;
    int            child;
	int				distance;

    vertex = 0;
    ant_farm->start->minpath = 0;
    while (ant_farm->rooms[vertex])
    {
        parent = 0;
        while (parent < ant_farm->verteces)
        {
            child = 0;
            while (child < ant_farm->verteces)
            {
                if (ant_farm->edges[parent][child] && !ant_farm->rooms[child]->visited)
			{
				if (!ant_farm->rooms[parent]->splitted || (ant_farm->rooms[parent]->splitted && ant_farm->rooms[parent]->path && ant_farm->rooms[parent]->path->splitted)
					|| (ant_farm->rooms[parent]->splitted && ant_farm->rooms[parent]->path && !ant_farm->rooms[parent]->path->splitted && ant_farm->edges[parent][child] < 0))
				{
					distance = ABS(ant_farm->edges[parent][child]) + ant_farm->rooms[parent]->minpath;
					if (ant_farm->rooms[child]->minpath > distance)
					{
						ant_farm->rooms[child]->minpath = distance;
					ant_farm->rooms[parent]->path = ant_farm->rooms[child];
					}
//						push(&q, ant_farm->rooms[child]);
//						q->top->visited = 1;
				}
			}
			++child;
            }
            ++parent;
        }
        ++vertex;
    }
	return (get_path(ant_farm));
}*/