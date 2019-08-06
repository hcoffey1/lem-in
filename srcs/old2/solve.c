/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   solve.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 23:53:44 by smorty            #+#    #+#             */
/*   Updated: 2019/08/06 20:11:10 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	clean_after_search(t_vertex **list, int size)
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
			if (colony->edges[path->top->index][path->next->top->index])
				++colony->edges[path->top->index][path->next->top->index]->paths;
			if (colony->edges[path->next->top->index][path->top->index])
				++colony->edges[path->next->top->index][path->top->index]->paths;
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
			if (colony->edges[i][j] && colony->edges[i][j]->paths != 1)
			{
				free(colony->edges[i][j]);
				colony->edges[i][j] = NULL;
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
			while (j < colony->verteces && !colony->edges[path->top->index][j])
			{
				++j;
				if (colony->rooms[j] == colony->start || (path->next && colony->rooms[j] == path->next->top))
					continue ;
			}
			if (path->top != colony->end)
			{
				free(colony->edges[path->top->index][j]);
				free(colony->edges[j][path->top->index]);
				colony->edges[path->top->index][j] = NULL;
				colony->edges[j][path->top->index] = NULL;
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
		if (colony->edges[colony->end->index][i])
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
		clean_after_search(colony->rooms, colony->verteces);
	}
	clean_after_search(colony->rooms, colony->verteces);
//	apply_paths(colony, path_list);
	return (pathfinding(colony));
}
