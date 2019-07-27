/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/27 15:34:50 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

/*
** type: -1 - unknown, 0 - connection, 1 - comment, 2 - vertex, 3 - start, 4 - end
*/

static t_vertex	*new_room(char *line, int type)
{
	t_vertex	*new;
	char	**split;

	if (type == 1)
		type = 2;
	if (!(new = (t_vertex *)malloc(sizeof(t_vertex))))
		error(-1);
	new->edges = NULL;
	new->type = type;
	new->visited = 0;
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
	return (new);
}

static int		check_line(char *line)
{
	int type;

	type = 0;
	if (*line == '#')
	{
		if (ft_strequ(line, "##start"))
			return (3);
		else if (ft_strequ(line, "##end"))
			return (4);
		else
			return (1);
	}
	else
		while (*line)
			if (*line++ == ' ')
				++type;
	if (type && type != 2)
		error(-1);
	return (type);
}

static void		new_link(t_edge **list, t_vertex *room1, t_vertex *room2)
{
	t_edge *new;

	while (*list)
	{
		if ((*list)->left == room1)
			(*list)->right = room2;
		else if ((*list)->left == room2)
			(*list)->right = room2;
		else if ((*list)->right == room1)
			(*list)->left = room2;
		else if ((*list)->right == room2)
			(*list)->left = room1;
		++list;
	}
	new->dir = 0;
	new->weight = 1;
	new->left = room1;
	new->right = room2;
	new->next = NULL;
}

static void		connect(t_vertex **rooms_list, char *line, int connects)
{
	t_vertex	**room1;
	t_vertex	**room2;
	char	**split;

	room1 = rooms_list;
	room2 = rooms_list;
	if (!(split = ft_strsplit(line, '-')))
		error(-1);
	if (!*split || !*(split + 1) || *(split + 2))
		error(-1);
	while (*room1 && !ft_strequ((*room1)->name, *split))
		++room1;
	if (!(*room1))
		error(-1);
	while (*room2 && !ft_strequ((*room2)->name, *(split + 1)))
		++room2;
	if (!*room1)
		error(-1);
	new_link(*room1, *room2);
	free(*split);
	free(*(split + 1));
	free(split);
}

t_vertex			**read_file(int fd, int verteces, int connects)
{
	char	*line;
	t_vertex	**rooms_list;
	t_vertex	*room;
	int		type;
	int		r;

	if ((r = get_next_line(fd, &line)) <= 0)
		error(-1);
	free(line);
	if (!(rooms_list = (t_vertex **)malloc(sizeof(t_vertex *) * (verteces + 1))))
		error(-1);
	*(rooms_list + verteces) = NULL;
	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			error(-1);
		if (*line == '#')
			type = check_line(line);
		else if (verteces)
		{
			rooms_list[--verteces] = new_room(line, type);
			type = 2;
		}
		else if (connects--)
			connect(rooms_list, line, connects--);
		free(line);
	}
	return (rooms_list);
}

void		validate(int fd, int *ants, int *verteces, int *connects)
{
	char	*line;
	int		r;
	int		start;
	int		end;

	if ((r = get_next_line(fd, &line)) <= 0)
		error(-1);
	*ants = ft_atoi(line);
	free(line);
	*verteces = 0;
	*connects = 0;
	start = 0;
	end = 0;
	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			error(-1);
		r = check_line(line);
		if (r > 2)
			r == 3 ? ++start : ++end;
		else if (r != 1)
			r == 2 ? ++(*verteces) : ++(*connects);
		free(line);
	}
	if (start != 1 || end != 1)
		error(-1);
	*connects *= 2;
}
