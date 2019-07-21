/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/21 17:25:39 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

/*
** type: -1 - unknown, 0 - connection, 1 - comment, 2 - vertex, 3 - start, 4 - end
*/

static t_room	*new_room(char *line, int type)
{
	t_room	*new;
	char	**split;

	if (!(new = (t_room *)malloc(sizeof(t_room))))
		exit(-1);
	new->links = NULL;
	new->type = type;
	new->edges = 0;
	if (!(split = ft_strsplit(line, ' ')))
		exit(-1);
	if (!*split || !*(split + 1) || !*(split + 2) || *(split + 3))
		exit(-1);
	new->name = ft_strdup(*split);
	free(*split);
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
		exit(-1);
	return (type);
}

static void		connect(t_room **rooms_list, char *line, int connects)
{
	t_room	**room1;
	t_room	**room2;
	char	**split;

	room1 = rooms_list;
	room2 = rooms_list;
	if (!(split = ft_strsplit(line, '-')))
		exit(-1);
	if (!*split || !*(split + 1) || *(split + 2))
		exit(-1);
	while (*room1 && !ft_strequ((*room1)->name, *split))
		++room1;
	if (!(*room1))
		exit(-1);
	while (*room2 && !ft_strequ((*room2)->name, *(split + 1)))
		++room2;
	if (!*room1)
		exit(-1);
	if (!(*room1)->links)
		(*room1)->links = malloc(sizeof(t_room *) * (connects + 1));
	(*room1)->links[(*room1)->edges++] = *room2;
	(*room1)->links[(*room1)->edges] = NULL;
	free(*split);
	free(*(split + 1));
	free(split);
}

t_room			**read_file(int fd, int verteces, int connects)
{
	char	*line;
	t_room	**rooms_list;
	t_room	*room;
	int		type;
	int		r;

	if ((r = get_next_line(fd, &line)) <= 0)
		exit(-1);
	rooms_list = (t_room **)malloc(sizeof(t_room *) * (verteces + 1));
	*(rooms_list + verteces) = NULL;
	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			exit(-1);
		if (*line == '#')
			type = check_line(line);
		else if (verteces)
			rooms_list[--verteces] = new_room(line, type);
		else if (connects)
			connect(rooms_list, line, connects--);
		free(line);
		type = 2;
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
		exit(-1);
	*ants = ft_atoi(line);
	free(line);
	*verteces = 0;
	*connects = 0;
	start = 0;
	end = 0;
	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			exit(-1);
		r = check_line(line);
		if (r > 2)
			r == 3 ? ++start : ++end;
		else if (r != 1)
			r == 2 ? ++(*verteces) : ++(*connects);
		free(line);
	}
	if (start != 1 || end != 1)
		exit(-1);
}

int main(int argc, char **argv)
{
	t_room **rooms_list;
	int		fd;
	int		ants;
	int		verteces;
	int		connects;

	if (argc != 2)
		return (0);
	fd = open(*(argv + 1), O_RDONLY);
	validate(fd, &ants, &verteces, &connects);
	close(fd);
	fd = open(*(argv + 1), O_RDONLY);
	rooms_list = read_file(fd, verteces, connects);
	int i = 0;
	while (i < verteces)
	{
		int j = 0;
		printf("%s ", rooms_list[i]->name);
		while (j < rooms_list[i]->edges)
			printf("%s ", rooms_list[i]->links[j++]->name);
		printf("\n");
		++i;
	}
	close(fd);
}