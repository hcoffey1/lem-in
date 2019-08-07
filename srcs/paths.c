/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/06 19:19:43 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 20:04:01 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	add_path(t_paths **path_list, t_queue *path, int len)
{
	t_paths	*new;

	if (!(new = (t_paths *)malloc(sizeof(t_paths))))
		error(errno);
	new->path = path;
	new->len = len;
	new->len0 = len;
	new->next = NULL;
	new->prev = NULL;
	if (*path_list)
	{
		while ((*path_list)->next)
			*path_list = (*path_list)->next;
		(*path_list)->next = new;
		new->prev = *path_list;
	}
	else
		*path_list = new;
}

void	print_paths(t_paths *path_list)
{
	t_queue *path;
	int		n;

	n = 0;
	while (path_list)
	{
		ft_printf("{yellow}Path %d, length {green}%d{eoc}:\n",
								++n, path_list->len0);
		path = path_list->path;
		while (path)
		{
			ft_putstr(path->top->name);
			ft_putchar(' ');
			path = path->next;
		}
		ft_putchar('\n');
		path_list = path_list->next;
	}
	ft_putchar('\n');
}

void	sort_paths(t_paths *list)
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

void	clear_paths(t_paths *path_list)
{
	t_queue *path;

	if (path_list)
	{
		while (path_list->next)
		{
			path = path_list->path;
			while (path->next)
			{
				path = path->next;
				free(path->prev);
			}
			free(path);
			path_list = path_list->next;
			free(path_list->prev);
		}
		path = path_list->path;
		while (path->next)
		{
			path = path->next;
			free(path->prev);
		}
		free(path);
		free(path_list);
	}
}
