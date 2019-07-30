/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 23:26:42 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	print_path(t_queue *path, int len)
{
	static int n = 0;

	ft_printf("{yellow}Path %d len %d:{eoc}\n", ++n, len);
	while (path)
	{
		ft_printf("%s ", path->top->name, path->top->splitted);
//		ft_printf("%d ", path->top->splitted);
		path = path->next;
	}
	ft_printf("\n");
}

void	apply_paths(t_lemin *colony, t_paths *path_list)
{
	t_queue	*path;
	int		i;
	int		j;

	i = 0;
	while (i < colony->verteces)
		ft_bzero(colony->edges[i++], sizeof(int) * colony->verteces);
	while (path_list)
	{
		path = path_list->path; 
		while (path->next)
		{
			++colony->edges[path->top->index][path->next->top->index];
			++colony->edges[path->next->top->index][path->top->index];
			path = path->next;
		}
		path_list = path_list->next;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = -1;
		while (++j < colony->verteces)
			if (colony->edges[i][j] == 2)
				colony->edges[i][j] = 0;
		++i;
	}
}

t_paths	*add_path(t_paths *path_list, t_queue *path, int len)
{
	t_paths *new;

	new = (t_paths *)malloc(sizeof(t_paths));
	new->path = path;
	new->len = len;
	new->prev = NULL;
	if ((new->next = path_list))
		new->next->prev = new;
	return (new);
}

t_queue *new_path_simple(t_lemin *colony, int *len)
{
	t_vertex	*track;
	t_queue		*path;

	path = new_queue(colony->start);
	track = colony->start->path;
	*len = 1;
	while (track)
	{
		colony->edges[path->top->index][track->index] = 0;
		colony->edges[track->index][path->top->index] = 0;
		push(&path, track);
		track = track->path;
		++(*len);
	}
	clear_path(colony->rooms, colony->verteces);
	while (path->prev)
		path = path->prev;
	return (path);
}

t_queue *bfs(t_lemin *colony, int *len)
{
	t_queue	*q;
	t_queue	*p;
	int		parent;
	int		child;

	q = new_queue(colony->end);
	colony->end->visited = 1;
	while (q->top != colony->start)
	{
		parent = q->top->index;
		child = 0;
		while (child < colony->verteces)
		{
			if (colony->edges[parent][child] && !colony->rooms[child]->visited)
			{
				colony->rooms[child]->path = colony->rooms[parent];
				colony->rooms[child]->visited = 1;
				push(&q, colony->rooms[child]);
			}
			++child;
		}
		pop(&q);
		if (!q)
			return (NULL);
	}
	while (q)
		pop(&q);
	return (new_path_simple(colony, len));
}

t_paths	*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;
	int		len;

	path_list = NULL;
	while ((path = dijkstra(colony)))
	{
		path_list = add_path(path_list, path, 0);
		print_path(path, 0);
	}
	apply_paths(colony, path_list);
	path_list = NULL;
	clear_path(colony->rooms, colony->verteces);
	int n = 0;
	while ((path = bfs(colony, &len)) && n < 10)
	{
		path_list = add_path(path_list, path, len);
		print_path(path, len);
		++n;
	}
	clear_path(colony->rooms, colony->verteces);
	return (path_list);
}
