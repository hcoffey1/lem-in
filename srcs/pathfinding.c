/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/07/31 23:59:08 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	print_path(t_queue *path, int len)
{
	static int n = 0;
	static int prev_len = 0;
	int s = 0;

	if (prev_len > len)
	{
		n = 0;
		prev_len = 0;
	}
	else
		prev_len = len;
	ft_printf("{yellow}Path %d len %d:{eoc}\n", ++n, len);
	while (path)
	{
		ft_printf("%s ", path->top->name, path->top->splitted);
//		ft_printf("%d ", path->top->splitted);
//		if (path->top->splitted)
//			ft_printf("%d\n", s);
//			++s;
		path = path->next;
	}
	ft_printf("\n");
}

void	apply_paths(t_lemin *colony, t_paths *path_list, int n)
{
	t_queue	*path;
	int		i;
	int		j;

	i = 0;
	while (i < colony->verteces)
		ft_bzero(colony->edges[i++], sizeof(int) * colony->verteces);
	i = 0;
	while (n--)
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
			if (colony->edges[i][j] >= 2)
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
	*len = 0;
	while (track)
	{
		colony->edges[path->top->index][track->index] = 0;
		colony->edges[track->index][path->top->index] = 0;
		push(&path, track);
		ft_printf("%s ", track->name);
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

	q = new_queue(colony->start);
	colony->start->closed = 1;
	while (q->top != colony->end)
	{
		q->top->closed = 1;
		parent = q->top->index;
		child = 0;
		while (child < colony->verteces)
		{
			if (colony->edges[parent][child] && !colony->rooms[child]->closed)
			{
				colony->rooms[parent]->path = colony->rooms[child];
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

int			check_path_set(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*set;
	int		len;

	set = NULL;
	while ((path = bfs(colony, &len)))
	{
		set = add_path(set, path, len);
	}
	return (open_the_gates(colony->ants, set));
}

void		evaluate_paths(t_lemin *colony, t_paths *path_list)
{
	t_paths	*p;
	int		n;
	int		i;
	int		optimal[100];

	n = 1;
	p = path_list;
	while (p)
	{
		apply_paths(colony, path_list, n);
		optimal[n] = check_path_set(colony);
		++n;
		p = p->next;
	}
	optimal[0] = INF_PATH;
	while (n--)
	{
		if (optimal[n] < optimal[0])
		{
			optimal[0] = optimal[n];
			i = n;
		}
	}
	ft_printf("{green}turns %d{eoc}\n", optimal[0]);
/*	while (i--)
		path_list = path_list->next;
	path_list->next = NULL;*/
}

t_paths	*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;
	int		len;
	int		n;
	int sum1;
	int sum2;

	path_list = NULL;
	n = 0;
	while ((path = dijkstra(colony, &len)) && n < 33)
	{
		path_list = add_path(path_list, path, len);
//		print_path(path, len);
		++n;
	}
	evaluate_paths(colony, path_list);
	clear_path(colony->rooms, colony->verteces);
	return (path_list);
}
