
#include "lemin.h"

static t_queue	*get_path_bellman(t_lemin *colony, int *len)
{
	t_vertex	*track;
	t_queue		*path;

	*len = 0;
	path = new_queue(colony->end);
	track = colony->end->path;
	while (track)
	{
		colony->edges[track->index][path->top->index] = 0;
		colony->edges[path->top->index][track->index] = 0;
		push_front(&path, track);
		track = track->path;
		++(*len);
	}
	clean_after_search(colony->rooms, colony->verteces);
	if (path->top != colony->start)
		return (NULL);
	ft_printf("{cyan}%d{eoc} ", *len);
	return (path);
}

t_queue		*bellman_ford(t_lemin *colony, int *len)
{
    int vertex;
    int parent;
    int child;
	int distance;

    vertex = 0;
    colony->start->minpath = 0;
	colony->start->closed = 1;
    while (vertex < colony->verteces)
    {
        parent = 0;
        while (parent < colony->verteces)
        {
            child = 0;
            while (child < colony->verteces)
            {
				if (colony->edges[parent][child] && !colony->rooms[child]->closed)
				{
					if (!colony->rooms[parent]->splitted
					|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path
						&& (colony->rooms[parent]->path->splitted
							|| colony->edges[parent][child] < 0)))
					{
						distance = colony->edges[parent][child] + colony->rooms[parent]->minpath;
						if (colony->rooms[child]->minpath > distance)
						{
							colony->rooms[child]->minpath = distance;
							colony->rooms[child]->path = colony->rooms[parent];
						}
						colony->rooms[child]->closed = 1;
					}
				}
				++child;
            }
            ++parent;
        }
        ++vertex;
    }
	return (get_path_bellman(colony, len));
}
