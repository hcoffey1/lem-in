/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   explore_anthill.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 20:51:55 by smorty            #+#    #+#             */
/*   Updated: 2019/09/05 18:17:38 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static void		clean_after_search(t_vertex **rooms, int verteces)
{
	while (verteces--)
		if (rooms[verteces])
		{
			rooms[verteces]->closed = 0;
			rooms[verteces]->path = NULL;
		}
}

static t_queue	*get_path(t_lemin *colony, int *len)
{
	t_vertex	*track;
	t_queue		*path;

	*len = 0;
	path = new_queue(colony->end);
	track = colony->end->path;
	while (track)
	{
		colony->edges[path->top->index][track->index] *= -1;
		track->splitted = 1;
		colony->edges[track->index][path->top->index] = 0;
		push_front(&path, track);
		track = track->path;
		++(*len);
	}
	colony->start->splitted = 0;
	while (path->prev)
		path = path->prev;
	clean_after_search(colony->rooms, colony->verteces);
	return (path);
}

static void		relax(t_lemin *colony, t_queue *q, int parent, int child)
{
	if (!colony->rooms[parent]->splitted
	|| (colony->rooms[parent]->splitted && colony->rooms[parent]->path
	&& (colony->rooms[parent]->path->splitted
	|| colony->edges[parent][child] < 0)))
	{
		colony->rooms[child]->path = colony->rooms[parent];
		push(&q, colony->rooms[child]);
		colony->rooms[child]->closed = 1;
	}
}

t_queue			*bfs(t_lemin *colony, int *len)
{
	t_queue	*q;
	int		parent;
	int		child;

	q = new_queue(colony->start);
	colony->start->closed = 1;
	while (q->top != colony->end)
	{
		parent = q->top->index;
		child = -1;
		while (++child < colony->verteces)
			if (colony->edges[parent][child] && !colony->rooms[child]->closed)
				relax(colony, q, parent, child);
		pop(&q);
		if (!q)
		{
			clean_after_search(colony->rooms, colony->verteces);
			return (NULL);
		}
	}
	while (q)
		pop(&q);
	return (get_path(colony, len));
}

t_paths			*explore_anthill(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;
	int		len;

	path_list = NULL;
	while ((path = bfs(colony, &len)))
		add_path(&path_list, path, len);
	if (!path_list)
		error(ERR_PATH);
	while (path_list->prev)
		path_list = path_list->prev;
	sort_paths(path_list);
	return (find_best_paths(colony, path_list));
}
