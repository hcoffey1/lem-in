/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_anthill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 17:29:39 by smorty            #+#    #+#             */
/*   Updated: 2019/08/04 23:47:09 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_vertex	*new_room(char *line, int verteces)
{
	static int	n = 0;
	t_vertex	*new;
	char		**split;

	if (!(new = (t_vertex *)malloc(sizeof(t_vertex))))
		error(errno);
	if (!(split = ft_strsplit(line, ' ')))
		error(errno);
	if (!*split || !*(split + 1) || !*(split + 2) || *(split + 3))
		error(ERR_ROOM);
	new->name = *split;
	new->x = ft_atoi(*(split + 1));
	free(*(split + 1));
	new->y = ft_atoi(*(split + 2));
	free(*(split + 2));
	free(split);
	new->index = n++;
	new->minpath = INF_PATH;
	new->splitted = 0;
	new->closed = 0;
	new->path = NULL;
	return (new);
}

static void		connect(int	**edges, t_vertex **rooms_list, char *line)
{
	char	**split;
	int			i1;
	int			i2;

	if (!(split = ft_strsplit(line, '-')))
		error(errno);
	if (!*split || !*(split + 1) || *(split + 2))
		error(ERR_PIPE);
	i1 = 0;
	i2 = 0;
	while (rooms_list[i1] && !ft_strequ(rooms_list[i1]->name, *split))
		++i1;
	while (rooms_list[i2] && !ft_strequ(rooms_list[i2]->name, *(split + 1)))
		++i2;
	if (!(rooms_list[i1] && rooms_list[i2]))
		error(ERR_PIPE);
	if (!(edges[i1][i2]))
		edges[i1][i2] = 1;
	if (!(edges[i2][i1]))
		edges[i2][i1] = 1;
	free(*split);
	free(*(split + 1));
	free(split);
}

t_vertex		**build_anthill(t_lemin *colony, t_mfile *map)
{
	t_vertex	**rooms;
	t_vertex	*new;

	if (!(rooms = (t_vertex **)malloc(sizeof(t_vertex *) * (colony->verteces + 1))))
		error(errno);
	rooms[colony->verteces] = NULL;
	while (map->type)
	{
		if (*map->line != '#')
		{
			new = new_room(map->line, colony->verteces);
			rooms[new->index] = new;
		}
		map = map->next;
	}
	while (map)
	{
		if (*map->line != '#')
			connect(colony->edges, rooms, map->line);
		map = map->next;
	}
	return (rooms);
}
