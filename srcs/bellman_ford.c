
#include "lemin.h"

void        relax(int **edges, t_vertex **rooms, int parent, int child)
{
    if (edges[parent][child] + rooms[parent]->minpath < rooms[child]->minpath)
	{
        rooms[child]->minpath = edges[parent][child] + rooms[parent]->minpath;
		rooms[child]->path = rooms[parent];
	}
}

t_queue		*bellman_ford_algorithm(t_lemin *ant_farm)
{
    int            vertex;
    int            parent;
    int            child;

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
                if (ant_farm->edges[parent][child])
                    relax(ant_farm->edges, ant_farm->rooms, parent, child);
                ++child;
            }
            ++parent;
        }
        ++vertex;
    }
	return (get_path(ant_farm));
}