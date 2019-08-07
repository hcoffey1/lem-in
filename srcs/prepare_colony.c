/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_colony.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/08/08 00:07:19 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_ants	*populate(t_vertex *start, int num)
{
	t_ants *first;
	t_ants **ant_ptr;

	first = NULL;
	ant_ptr = &first;
	while (num)
	{
		if (!(*ant_ptr = (t_ants *)malloc(sizeof(t_ants))))
			error(errno);
		(*ant_ptr)->num = num;
		(*ant_ptr)->room = start;
		(*ant_ptr)->next = NULL;
		ant_ptr = &(*ant_ptr)->next;
		--num;
	}
	return (first);
}

static void		find_ends(t_lemin *colony, t_input *map)
{
	int index;

	index = 0;
	while (map && map->type != LINK)
	{
		if (map->type == START)
			colony->start = colony->rooms[index];
		else if (map->type == END)
			colony->end = colony->rooms[index];
		if (map->type > COMMENT)
			++index;
		map = map->next;
	}
}

static void		check_duplicates(t_vertex **rooms)
{
	t_vertex **p;

	while (*rooms)
	{
		p = rooms;
		while (*++p)
			if (ft_strequ((*rooms)->name, (*p)->name)
			|| ((*rooms)->x == (*p)->x && (*rooms)->y == (*p)->y))
				error(ERR_DUPE);
		++rooms;
	}
}

static int		**edges_matrix(int verteces)
{
	int **matrix;
	int i;

	if (!(matrix = (int **)malloc(sizeof(int *) * (verteces + 1))))
		error(errno);
	i = 0;
	while (i < verteces)
	{
		if (!(matrix[i] = (int *)malloc(sizeof(int) * verteces)))
			error(errno);
		ft_bzero(matrix[i], sizeof(int) * verteces);
		++i;
	}
	matrix[i] = NULL;
	return (matrix);
}

t_lemin			*prepare_colony(t_input *map)
{
	t_lemin *colony;

	if (!(colony = (t_lemin *)malloc(sizeof(t_lemin))))
		error(errno);
	validate(map, &colony->ants_num, &colony->verteces);
	colony->edges = edges_matrix(colony->verteces);
	colony->rooms = build_anthill(map, colony->edges, colony->verteces);
	check_duplicates(colony->rooms);
	colony->start = NULL;
	colony->end = NULL;
	find_ends(colony, map);
	colony->ants = populate(colony->start, colony->ants_num);
	return (colony);
}
