/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 23:53:44 by smorty            #+#    #+#             */
/*   Updated: 2019/07/29 20:10:58 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	clear_path(t_vertex **list, int size)
{
	while (size--)
	{
		list[size]->minpath = INF_PATH;
		list[size]->path = NULL;
		list[size]->visited = 0;
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

void		apply_paths(t_lemin *colony, t_paths *path_list)
{
	t_queue	*path;
	int		i;
	int		j;

	while (path_list)
	{
		path = path_list->path;
		while (path->next)
		{
			if (colony->adjacency[path->top->index][path->next->top->index])
				++colony->adjacency[path->top->index][path->next->top->index]->paths;
			if (colony->adjacency[path->next->top->index][path->top->index])
				++colony->adjacency[path->next->top->index][path->top->index]->paths;
			path = path->next;
		}
		path_list = path_list->next;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
		{
			if (colony->adjacency[i][j] && colony->adjacency[i][j]->paths != 1)
			{
				free(colony->adjacency[i][j]);
				colony->adjacency[i][j] = NULL;
			}
			++j;
		}
		++i;
	}
}

void		finish_paths(t_lemin *colony, t_paths *path_list)
{
	t_queue *path;
	int i;
	int j;
	int n = 0;

	while (path_list)
	{
		path = path_list->path;
		ft_printf("{cyan}%d {eoc}", ++n);
		while (path->top != colony->end)
		{
//			if (path->top != colony->end && path->top != colony->start)
//				path->top->visited = 1;
			ft_printf("%s ", path->top->name);
			j = 0;
			while (j < colony->verteces && !colony->adjacency[path->top->index][j])
			{
				++j;
				if (colony->rooms[j] == colony->start || (path->next && colony->rooms[j] == path->next->top))
					continue ;
			}
			if (path->top != colony->end)
			{
				free(colony->adjacency[path->top->index][j]);
				free(colony->adjacency[j][path->top->index]);
				colony->adjacency[path->top->index][j] = NULL;
				colony->adjacency[j][path->top->index] = NULL;
			}
/*			if (colony->rooms[j]->visited)
			{
				path_list->path = NULL;
				path_list->len = INF_PATH;
				break ;
			}
			else
			{*/
				push(&path, colony->rooms[j]);
				++path_list->len;
//			}
		}
//		ft_printf("%s", path->top->name);
		ft_printf("\n");
		path_list = path_list->next;
	}
}

t_paths		*pathfinding(t_lemin *colony)
{
	t_paths	*path_list;
	t_vertex *t;
	int		i;
	int n = 0;

	i = 0;
	while (i < colony->verteces)
	{
		if (colony->adjacency[colony->end->index][i])
			path_list = add_path(path_list, new_queue(colony->start), 0);
		++i;
	}
	finish_paths(colony, path_list);
	return (path_list);
}

t_paths		*solve(t_lemin *colony)
{
	t_paths *path_list;
	t_queue	*path;

	path_list = NULL;
	while ((path = dijkstra_algorithm(colony)))
	{
		path_list = add_path(path_list, path, 0);
		clear_path(colony->rooms, colony->verteces);
	}
	clear_path(colony->rooms, colony->verteces);
//	apply_paths(colony, path_list);
	return (pathfinding(colony));
}
