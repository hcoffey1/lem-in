/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prepare_colony.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/08/05 00:28:53 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

/*
** type: 0 - connection, 1 - comment, 2 - vertex, 3 - start, 4 - end
*/

int			check_line(char *line)
{
	int type;

	type = 0;
	if (*line == '#')
	{
		if (ft_strequ(line, "##start"))
			return (3);
		else if (ft_strequ(line, "##end"))
			return (4);
		return (1);
	}
	while (*line)
		if (*line++ == ' ')
			++type;
	if (type && type != 2)
		error(ERR_LINE);
	return (type);
}

static void	validate(t_mfile *map, int *ants, int *verteces)
{
	int type[5];

	*ants = ft_atoi(map->line);
	if (!ft_strequ(map->line, ft_itoa(*ants)))
		error(ERR_ANTS);
	ft_bzero(type, sizeof(int) * 5);
	map = map->next;
	while (map && !type[0])
	{
		map->type = check_line(map->line);
		++type[map->type];
		map = map->next;
	}
	if (type[3] != 1 || type[4] != 1)
		error(ERR_STEN);
	*verteces = type[2] + type[3] + type[4];
	while (map)
	{
		map->type = check_line(map->line);
		++type[map->type];
		map = map->next;
	}
	if (*verteces != type[2] + type[3] + type[4])
		error(ERR_PIPE);
}

static int	**edges_matrix(int verteces)
{
	int	**matrix;
	int	i;

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

static void	find_ends(t_lemin *colony, t_mfile *map)
{
	int index;

	index = 0;
	while (map)
	{
		if (map->type > 1)
		{
			if (map->type == 3)
				colony->start = colony->rooms[index];
			else if (map->type == 4)
				colony->end = colony->rooms[index];
			else
				++index;
		}
		map = map->next;
	}
	if (!colony->start || !colony->end)
		error(ERR_STEN);
}

t_lemin		*prepare_colony(t_mfile *map)
{
	t_lemin	*colony;

	if (!(colony = (t_lemin *)malloc(sizeof(t_lemin))))
		error(errno);
	validate(map, &colony->ants_num, &colony->verteces);
	colony->edges = edges_matrix(colony->verteces);
	colony->rooms = build_anthill(colony, map->next);
	find_ends(colony, map);
	colony->ants = populate(colony->start, colony->ants_num);
	return (colony);
}
