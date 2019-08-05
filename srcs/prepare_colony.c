/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_colony.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/08/05 23:46:39 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

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

static t_ants	*populate(t_vertex *start, int num)
{
	static int	n = 0;
	t_ants		*ants;

	if (!num)
		return (NULL);
	if (!(ants = (t_ants *)malloc(sizeof(t_ants))))
		error(errno);
	ants->num = ++n;
	ants->room = start;
	ants->path = NULL;
	ants->next = populate(start, --num);
	return (ants);
}

static void		find_ends(t_lemin *colony, t_mfile *map)
{
	int index;

	index = 0;
	while (map->type != LINK)
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

t_lemin			*prepare_colony(t_mfile *map)
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
