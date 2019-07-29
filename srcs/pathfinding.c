/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/07/30 00:22:55 by smorty           ###   ########.fr       */
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
		ft_bzero(colony->adjacency[i++], sizeof(int) * colony->verteces);
	while (path_list)
	{
		path = path_list->path;
		while (path->next)
		{
			if (path->top->index > colony->verteces / 2)
				path->top->index = colony->verteces - path->top->index;
			ft_printf("%s %d - ", path->top->name, path->top->index);
			++colony->adjacency[path->top->index][path->next->top->index];
			path = path->next;
		}
		ft_printf("\n");
		path_list = path_list->next;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
			ft_printf("%d ", colony->adjacency[i][j++]);
		++i;
		ft_printf("\n");
	}
		ft_printf("\n");
	i = 0;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
		{
			if (colony->adjacency[i][j] && colony->adjacency[j][i])
			{
				colony->adjacency[i][j] = 0;
				colony->adjacency[j][i] = 0;
			}
			++j;
		}
		++i;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = 0;
		while (j < colony->verteces)
			ft_printf("%d ", colony->adjacency[i][j++]);
		++i;
		ft_printf("\n");
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

t_paths	*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;

	path_list = NULL;
	while ((path = dijkstra(colony, 1)))
	{
		path_list = add_path(path_list, path, 0);
		print_path(path);
	}
	apply_paths(colony, path_list);
	path_list = NULL;
	int n = 0;
	while ((path = dijkstra(colony, 0)) && n < 10)
	{
		path_list = add_path(path_list, path, 0);
		print_path(path);
		++n;
	}
	return (path_list);
}