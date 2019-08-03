/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   march.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/08/03 23:59:22 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_queue	*shortest_path(t_paths *path_list)
{
	t_paths	*shortest;
	int		len;

	len = INF_PATH;
	while (path_list)
	{
		if (path_list->len < len)
		{
			len = path_list->len;
			shortest = path_list;
		}
		path_list = path_list->next;
	}
	if (shortest->len)
		++shortest->len;
	return (shortest->path);
}

static void		distribute_paths(t_ants *ants, t_paths *path_list)
{
	while (ants)
	{
		ants->path = shortest_path(path_list);
		ants->room = ants->path->top;
//		ft_printf("%d ", ants->path->next->top->index);
		ants = ants->next;
	}
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

int				open_the_gates(t_lemin *colony, t_paths *path_list, int flag)
{
	t_ants	*ants;
	int		num;

	distribute_paths(colony->ants, path_list);
	num = 0;
	while (!all_gone(colony->ants, colony->end)) // пока все не находятся в последней комнате
	{
		++num;
//		ft_printf("{magenta}turn %3d:\n{eoc}", num); // remove
		ants = colony->ants;
		while (ants) // для каждого муравья
		{
			if (ants->room != colony->end && !ants->path->next->top->closed) // если текущая комната муравья не последняя и следующая не занята
			{
				if (flag)
					ft_printf("L%d-", ants->num);
				ants->room->closed = 0; // открываем текующую комнату
				ants->path = ants->path->next; // переходим к следующей
				ants->room = ants->path->top;
				if (ants->room != colony->end) // и закрываем её
					ants->room->closed = 1;
				if (flag)
					ft_printf("%s ", ants->room->name);
			}
			ants = ants->next;
		}
		if (flag)
			ft_printf("\n");
	}
	ft_printf("{cyan}%d{eoc}\n", num);
	return (num);
}
