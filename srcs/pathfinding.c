/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/08/01 23:37:56 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void		clean_after_search(t_vertex **rooms, int verteces)
{
	while (verteces--)
		if (rooms[verteces])
		{
			rooms[verteces]->closed = 0;
			rooms[verteces]->minpath = INF_PATH;
			rooms[verteces]->path = NULL;
		}
}

void		apply_paths(t_lemin *colony, t_paths *path_list)
{
	static int	n = 0;
	t_queue		*path;
	int			i;
	int			j;

	i = 0;
	while (i < colony->verteces)
		ft_bzero(colony->edges[i++], sizeof(int) * colony->verteces);
	i = ++n;
	while (i--)
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

void		add_path(t_paths **path_list, t_queue *path, int len)
{
	t_paths	*new;

	new = (t_paths *)malloc(sizeof(t_paths));
	new->path = path;
	new->len = len;
	new->len0 = len;
	new->next = NULL;
	if (*path_list)
	{
		while ((*path_list)->next)
			*path_list = (*path_list)->next;
		(*path_list)->next = new;
		new->prev = *path_list;
	}
	else
	{
		new->prev = NULL;
		*path_list = new;
	}
}

int			approx_turns(int ants, t_paths *set)
{
	t_paths	*set0;
	t_paths	*set1;
	
	if (!set->next)
		return (set->len + ants - 1);
	if (!set->len)
		return (0);
	set0 = set;
	set1 = set->next;
	while (ants > 0)
	{
		while (set->len < set1->len)
		{
			while (set != set1)
			{
				++set->len;
				--ants;
				set = set->next;
			}
			set = set0;
		}
		if (set1->next)
			set1 = set1->next;
		else
		{
			++set1->len;
			--ants;
		}
		set = set0;
	}
	return (set0->len);
}

int			check_paths_set(t_lemin *colony, t_paths **set)
{
	t_queue	*path;
	int		len;

	*set = NULL;
	while ((path = bfs(colony, &len)))
		add_path(set, path, len);
	clean_after_search(colony->rooms, colony->verteces);
	if (!*set)
		return (INF_PATH);
	while ((*set)->prev)
		*set = (*set)->prev;
	return (approx_turns(colony->ants_num, *set));
}

void		clear_paths(t_paths *path_list)
{
	t_queue *path;

	if (path_list && path_list->next)
	{
		path_list = path_list->next;
		path_list->prev->next = NULL;
		while (path_list->next)
		{
			path = path_list->path->next;
			while (path->next)
			{
				free(path->prev);
				path = path->next;
			}
			free(path);
			path_list = path_list->next;
			free(path_list->prev);
		}
	}
	free(path_list);
}

t_paths		*evaluate_paths(t_lemin *colony, t_paths *path_list)
{
	t_paths	*p;
	t_paths	*prev;
	t_paths	*curr;
	int		prev_len;
	int		curr_len;

	p = path_list;
	prev_len = INF_PATH;
	curr = NULL;
	prev = NULL;
	while (p)
	{
		apply_paths(colony, path_list);
		if ((curr_len = check_paths_set(colony, &curr)) > prev_len)
		{
			clear_paths(curr);
			clear_paths(path_list);
			return (prev);
		}
		clear_paths(prev);
		prev = curr;
		prev_len = curr_len;
		p = p->next;
	}
	clear_paths(path_list);
	return (curr);
}

t_paths		*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;
	int		len;

	path_list = NULL;
	while ((path = dijkstra(colony, &len)))
		add_path(&path_list, path, len);
	clean_after_search(colony->rooms, colony->verteces);
	while (path_list->prev)
		path_list = path_list->prev;
	return (evaluate_paths(colony, path_list));
}
