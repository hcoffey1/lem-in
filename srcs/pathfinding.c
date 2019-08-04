/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pathfinding.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/29 21:17:41 by smorty            #+#    #+#             */
/*   Updated: 2019/08/04 18:27:25 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void		sort_list(t_paths *list)
{
	t_paths *left;
	t_paths	*right;
	t_paths *list0;

	list0 = list;
	while (list->next)
	{
		if (list->len > list->next->len)
		{
			left = list->prev;
			right = list->next->next;
			if (left)
				left->next = list->next;
			if (right)
				right->prev = list;
			list->next->next = list;
			list->next->prev = left;
			list->prev = list->next;
			list->next = right;
			list = list0;
		}
		list = list->next;
	}
}

void		print_path(t_queue *path)
{
	while (path)
	{
		ft_printf("%s ", path->top->name);
		path = path->next;
	}
	ft_printf("\n");
}

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

void			apply_paths(t_lemin *colony, t_paths *path_list)
{
	static int	n = 0;
	t_queue		*path;
	int			i;
	int			j;

	i = 0;
	while (i < colony->verteces)
		ft_bzero(colony->edges[i++], sizeof(int) * colony->verteces);
	i = ++n;
	while (i-- && path_list)
	{
		path = path_list->path;
		while (path->next)
		{
			++colony->edges[path->top->index][path->next->top->index];
			path->top->splitted = 0;
			path = path->next;
		}
		path_list = path_list->next;
	}
	i = 0;
	while (i < colony->verteces)
	{
		j = -1;
		while (++j < colony->verteces)
			if (colony->edges[i][j] && colony->edges[j][i])
			{
				colony->edges[i][j] = 0;
				colony->edges[j][i] = 0;
			}
		++i;
	}
}

void		add_path(t_paths **path_list, t_queue *path, int len)
{
	t_paths	*new;

	if (!(new = (t_paths *)malloc(sizeof(t_paths))))
		error(errno);
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

void		print_lens(t_paths *list)
{
	while (list)
	{
		ft_printf("{yellow}%d {eoc}", list->len);
		list = list->next;
	}
	ft_printf("\n");
}

int			check_paths_set(t_lemin *colony, t_paths **set)
{
	t_queue		*path;
	int			len;

	*set = NULL;
	while ((path = bfs(colony, &len)))
		add_path(set, path, len);
	clean_after_search(colony->rooms, colony->verteces);
	while ((*set)->prev)
		*set = (*set)->prev;
	return (open_the_gates(colony, *set, 0));
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

void		restore_lens(t_paths *set)
{
	while (set)
	{
		set->len = set->len0;
		set = set->next;
	}
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
		if ((curr_len = check_paths_set(colony, &curr)) > prev_len + 100)
		{
			clear_paths(curr);
			clear_paths(path_list);
			restore_lens(prev);
			return (prev);
		}
		clear_paths(prev);
		prev = curr;
		prev_len = curr_len;
		p = p->next;
	}
	clear_paths(path_list);
	restore_lens(curr);
	return (curr);
}

t_paths		*find_paths(t_lemin *colony)
{
	t_queue	*path;
	t_paths	*path_list;
	int		len;

	path_list = NULL;
	while ((path = bfs(colony, &len)))
		add_path(&path_list, path, len);
	clean_after_search(colony->rooms, colony->verteces);
	while (path_list->prev)
		path_list = path_list->prev;
	sort_list(path_list);
	clean_after_search(colony->rooms, colony->verteces);
	return (evaluate_paths(colony, path_list));
}
