/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/27 17:29:39 by smorty            #+#    #+#             */
/*   Updated: 2019/08/03 22:09:02 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static t_vertex	*new_room(char *line, int verteces)
{
	static int	n = 0;
	t_vertex	*new;
	char		**split;

	if (!(new = (t_vertex *)malloc(sizeof(t_vertex))))
		error(-1);
	if (!(split = ft_strsplit(line, ' ')))
		error(-1);
	if (!*split || !*(split + 1) || !*(split + 2) || *(split + 3))
		error(-1);
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
		error(-1);
	if (!*split || !*(split + 1) || *(split + 2))
		error(-1);
	i1 = 0;
	i2 = 0;
	while (rooms_list[i1] && !ft_strequ(rooms_list[i1]->name, *split))
		++i1;
	while (rooms_list[i2] && !ft_strequ(rooms_list[i2]->name, *(split + 1)))
		++i2;
	if (!(rooms_list[i1] && rooms_list[i2]))
		error(-1);
	if (!(edges[i1][i2]))
		edges[i1][i2] = 1;
	if (!(edges[i2][i1]))
		edges[i2][i1] = 1;
	free(*split);
	free(*(split + 1));
	free(split);
}

t_vertex		**process_file(t_lemin *colony, int fd)
{
	char		*line;
	t_vertex	**rooms;
	t_vertex	*new;
	int			type;
	int			verteces;

	if (get_next_line(fd, &line) <= 0)
		error(-1);
	free(line);
	verteces = colony->verteces;
	if (!(rooms = (t_vertex **)malloc(sizeof(t_vertex *) * verteces)))
		error(-1);
	type = 2;
	while (verteces)
	{
		if (get_next_line(fd, &line) <= 0)
			error(-1);
		if (*line == '#')
			type = check_line(line);
		else
		{
			new = new_room(line, colony->verteces);
			if (type == 3)
				colony->start = new;
			else if (type == 4)
				colony->end = new;
			rooms[new->index] = new;
			--verteces;
			type = 2;
		}
		free(line);
	}
	while ((type = get_next_line(fd, &line)))
	{
		if (type < 0)
			error(-1);
		if (*line != '#')
			connect(colony->edges, rooms, line);
		free(line);
	}
	return (rooms);
}
