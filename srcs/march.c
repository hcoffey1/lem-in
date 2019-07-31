/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   march.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/23 22:34:43 by smorty            #+#    #+#             */
/*   Updated: 2019/07/31 23:56:32 by smorty           ###   ########.fr       */
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
		ants = ants->next;
	}
}

static int		all_gone(t_ants *ants)
{
	while (ants)
	{
		if (ants->room->type != 4)
			return (0);
		ants = ants->next;
	}
	return (1);
}

int		open_the_gates(t_ants *ants, t_paths *path_list)
{
	t_ants *ants0;
	int num = 0;

	distribute_paths(ants, path_list);
	ants0 = ants;
	while (!all_gone(ants0)) // пока все не находятся в последней комнате
	{
//		ft_printf("{magenta}turn %3d: {eoc}", ++num); // remove
		++num;
		ants = ants0;
		while (ants) // для каждого муравья
		{
			if (ants->room->type != 4 && !ants->path->next->top->closed) // если текущая комната муравья не последняя и следующая не занята
			{
//				ft_printf("L%d-", ants->num);
				ants->room->closed = 0; // открываем текующую комнату
				ants->path = ants->path->next; // переходим к следующей
				ants->room = ants->path->top;
				if (ants->room->type != 4) // и закрываем её
					ants->room->closed = 1;
//				ft_printf("%s ", ants->room->name);
			}
			ants = ants->next;
		}
	}
	ft_printf("%d\n", num);
	return (num);
}
