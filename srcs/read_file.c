/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/19 00:03:42 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

/*
** type: -1 - unknown, 0 - connection, 1 - comment, 2 - vertex, 3 - start, 4 - end
*/

static t_room	*read_line(t_room *rooms, char *line, int type)
{
	t_room	*new;
	char	**split;

	if (!(new = (t_room *)malloc(sizeof(t_room))))
		exit(-1);
	new->links = NULL;
	new->right = NULL;
	if ((new->left = rooms))
		rooms->right = new;
	new->type = type;
	new->edges = 0;
	if (!(split = ft_strsplit(line, ' ')))
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
			return(1);
	}
	else
		while (*line)
			if (*line++ == ' ')
				++type;
	if (type && type != 2)
		exit(-1);
	return (type);
}

static void		connect(t_room *rooms1, char *line, int count)
{
	t_room	*rooms2;
	char	**split;

	rooms2 = rooms1;
	if (!(split = ft_strsplit(line, '-')))
		exit(-1);
	if (!*split || !*(split + 1) || *(split + 2))
		exit(-1);
	while (rooms1 && !ft_strequ(rooms1->name, *split))
		rooms1 = rooms1->left;
	if (!rooms1)
		exit(-1);
	while (rooms2 && !ft_strequ(rooms2->name, *(split + 1)))
		rooms2 = rooms2->left;
	if (!rooms2)
		exit(-1);
	if (!rooms1->links)
		rooms1->links = malloc(sizeof(t_room *) * (count + 1));
	rooms1->links[rooms1->edges++] = (t_room *)rooms2;
	rooms1->links[rooms1->edges] = NULL;
	free(*split);
	free(*(split + 1));
	free(split);
}

t_room			*read_file(int fd, int *ants)
{
	char	*line;
	int		r;
	int		type;
	t_room	*rooms;
	int		count;

	rooms = NULL;
	r = get_next_line(fd, &line);
	*ants = ft_atoi(line);
	free(line);
	count = 0;
	type = -1;
	while ((r = get_next_line(fd, &line)))
	{
		if (r < 0)
			exit(-1);
		if (type)
		{
			type = check_line(line);
			if (type == 2)
			{
				rooms = read_line(rooms, line, type);
				++count;
				type = -1;
			}
		}
		else if (*line != '#')
			connect(rooms, line, count);
		free(line);
	}
	while (rooms->left)
		rooms = rooms->left;
	return (rooms);
}

int main(int argc, char **argv)
{
	t_room *rooms;
	int		fd;
	int		ants;

	if (argc != 2)
		return (0);
	fd = open(*(argv + 1), O_RDONLY);
	rooms = read_file(fd, &ants);
	close(fd);
}