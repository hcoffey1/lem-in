/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_the_gates.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 22:45:18 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_queue	*shortest_path(t_paths *path_list)
{
	while (path_list && path_list->next
			&& path_list->len >= path_list->next->len)
		path_list = path_list->next;
	if (path_list->len)
		++path_list->len;
	return (path_list->path);
}

static void		distribute_paths(t_ants *ants, t_paths *path_list, int flags)
{
	t_paths *p;

	if (flags)
	{
		p = path_list;
		while (p)
		{
			p->len = p->len0;
			p = p->next;
		}
	}
	while (ants)
	{
		ants->path = shortest_path(path_list);
		ants->room = ants->path->top;
		ants = ants->next;
	}
}

static void		move_ants(t_ants *ants, t_vertex *end, int flags)
{
	int space;

	space = -1;
	while (ants)
	{
		if (ants->room != end && !ants->path->next->top->closed)
		{
			if ((flags & F_FULL) && ++space)
				ft_putchar(' ');
			ants->room->closed = 0;
			ants->path = ants->path->next;
			ants->room = ants->path->top;
			if (ants->room != end)
				ants->room->closed = 1;
			if (flags & F_FULL)
				ft_printf("L%d-%s", ants->num, ants->room->name);
		}
		ants = ants->next;
	}
	if (flags & F_FULL)
		ft_putchar('\n');
}

static int		all_gone(t_ants *ants, t_vertex *end)
{
	while (ants)
	{
		if (ants->room != end)
			return (0);
		ants = ants->next;
	}
	return (1);
}

int				open_the_gates(t_lemin *colony, t_paths *path_list, int flags)
{
	int num;

	distribute_paths(colony->ants, path_list, flags);
	num = 0;
	while (!all_gone(colony->ants, colony->end))
	{
		++num;
		if ((flags & F_DEBUG) && (flags & F_FULL))
			ft_printf("{magenta}Turn %d:\n{eoc}", num);
		move_ants(colony->ants, colony->end, flags);
	}
	if (flags & F_TURNS)
		ft_printf("Total turns: {green}%d{eoc}\n", num);
	return (num);
}
