/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_anthill.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 17:29:39 by smorty            #+#    #+#             */
/*   Updated: 2019/09/04 19:38:04 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	connect(char *line, int **edges, t_vertex **rooms_list)
{
	char	**split;
	int		i1;
	int		i2;

	if (!(split = ft_strsplit(line, '-')))
		error(errno);
	i1 = 0;
	i2 = 0;
	while (rooms_list[i1] && !ft_strequ(rooms_list[i1]->name, split[0]))
		++i1;
	while (rooms_list[i2] && !ft_strequ(rooms_list[i2]->name, split[1]))
		++i2;
	free(split[0]);
	free(split[1]);
	free(split);
	if (!rooms_list[i1] || !rooms_list[i2])
		return (0);
	edges[i1][i2] = 1;
	edges[i2][i1] = 1;
	return (1);
}

static void	linking(t_input *map, int **edges, t_vertex **rooms)
{
	while (map)
	{
		if (map->type != COMMENT
			&& (map->type != LINK || !connect(map->line, edges, rooms)))
		{
			map->prev->next = NULL;
			while (map->next)
			{
				free(map->line);
				map = map->next;
				free(map->prev);
			}
			free(map->line);
			free(map);
			return ;
		}
		map = map->next;
	}
}

static void	check_coords(char *x_line, char *y_line, int x, int y)
{
	char *check;

	check = ft_itoa(x);
	if (!ft_strequ(x_line, check))
		error(ERR_ROOM);
	free(check);
	check = ft_itoa(y);
	if (!ft_strequ(y_line, check))
		error(ERR_ROOM);
	free(check);
}

static void	*new_room(char *line, t_vertex **rooms)
{
	static int	n = 0;
	t_vertex	*new;
	char		**split;

	if (!(new = (t_vertex *)malloc(sizeof(t_vertex))))
		error(errno);
	if (!(split = ft_strsplit(line, ' ')))
		error(errno);
	if (!split[0] || *split[0] == 'L' || !split[1] || !split[2] || split[3])
		error(ERR_ROOM);
	new->name = split[0];
	new->x = ft_atoi(split[1]);
	new->screen_x = 1.0;
	new->y = ft_atoi(split[2]);
	new->screen_y = 1.0;
	check_coords(split[1], split[2], new->x, new->y);
	free(split[1]);
	free(split[2]);
	free(split);
	rooms[n] = new;
	new->index = n++;
	new->splitted = 0;
	new->closed = 0;
	new->path = NULL;
	return (new);
}

t_vertex	**build_anthill(t_input *map, int **edges, int verteces)
{
	t_vertex **rooms;

	while (map->type == COMMENT || !map->type)
		map = map->next;
	if (!(rooms = (t_vertex **)malloc(sizeof(t_vertex *) * (verteces + 1))))
		error(errno);
	rooms[verteces] = NULL;
	while (map->type != LINK)
	{
		if (map->type != COMMENT)
			new_room(map->line, rooms);
		map = map->next;
	}
	linking(map, edges, rooms);
	return (rooms);
}
