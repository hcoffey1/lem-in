/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 19:17:44 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	print_path(t_queue *path)
{
	static int n = 0;

	ft_printf("{yellow}Path %d:{eoc}\n", ++n);
	while (path)
	{
		ft_printf("%s ", path->top->name);
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
			if (path->top->name == path->next->top->name)
				++colony->edges[path->top->index][path->next->top->index];

			++colony->edges[path->top->index][path->next->top->index];
			path = path->next;
		}
		path_list = path_list->next;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
			ft_printf("%d ", colony->edges[i][j++]);
		++i;
		ft_printf("\n");
	}
	i = 0;
/*	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
		{
			if (colony->edges[i][j] && colony->edges[j][i])
			{
				colony->edges[i][j] = 0;
				colony->edges[j][i] = 0;
			}
			++j;
		}
		++i;
	}*/
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
/*
t_queue *new_path_simple(t_lemin *colony)
{
	t_vertex	*track;
	t_queue		*path;

	path = new_queue(colony->end);
	track = colony->end->path;
	while (path)
	{
		push_front(&path, track);
		track = track->path;
	}
	clear_path(colony->rooms, colony->verteces);
	return (path);
}

t_queue *bfs(t_lemin *colony)
{
	t_queue	*q;
	int		parent;
	int		child;

	q = new_queue(colony->end);
	colony->start->minpath = 0;
	colony->start->visited = 1;
	while (q->top != colony->start)
	{
		parent = q->top->index;
		child = 0;
		while (child < colony->verteces)
		{
			if (colony->edges[parent][child] && !colony->rooms[child]->visited)
			{
				if (colony->rooms[child]->minpath > colony->rooms[parent]->minpath + colony->edges[parent][child])
				{
					colony->rooms[child]->minpath = colony->rooms[parent]->minpath + colony->edges[parent][child];
					colony->rooms[child]->path = colony->rooms[parent];
				}
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
	return (new_path_simple(colony));
}*/

t_paths	*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;

	path_list = NULL;
	while ((path = dijkstra(colony)))
	{
		path_list = add_path(path_list, path, 0);
		print_path(path);
	}
/*	while ((path = bellman_ford_algorithm(colony)))
	{
		path_list = add_path(path_list, path, 0);
		print_path(path);
	}*/
//	apply_paths(colony, path_list);
	path_list = NULL;
	int n = 0;
/*	while ((path = bfs(colony)) && n < 10)
	{
		path_list = add_path(path_list, path, 0);
		print_path(path);
		++n;
	}*/
	return (path_list);
}
