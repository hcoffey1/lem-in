/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colonize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/27 18:34:04 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

/*
** type: -1 - unknown, 0 - connection, 1 - comment, 2 - vertex, 3 - start, 4 - end
*/

int				check_line(char *line)
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

static void		validate(int fd, int *ants, int *verteces, int *edges)
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
	*edges = 0;
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
			r == 2 ? ++(*verteces) : ++(*edges);
		free(line);
	}
	if (start != 1 || end != 1)
		error(-1);
	*edges *= 2;
}

static t_edge	***adjacency_matrix(int edges)
{
	t_edge	***matrix;
	int i;

	if (!(matrix = (t_edge ***)malloc(sizeof(t_edge **) * (edges + 1))))
		error(-1);
	i = 0;
	while (i < edges)
	{
		matrix[i] = (t_edge **)malloc(sizeof(t_edge *) * (edges + 1));
		ft_bzero(matrix[i], sizeof(matrix[i]) + sizeof(t_edge));
		++i;
	}
	matrix[i] = NULL;
	return (matrix);
}

static t_ants	*populate_ants(t_vertex *start, int num)
{
        t_ants          *ants;
        static int      n = 0;

        if (!num)
                return (NULL);
        ants = (t_ants *)malloc(sizeof(t_ants));
        ants->num = ++n;
        ants->room = start;
        ants->path = NULL;
        ants->next = populate_ants(start, --num);
        return (ants);
}

t_lemin			*create_ant_colony(char *filename)
{
	t_lemin	*colony;
	int		fd;
	int		ants_num;
	int		verteces;
	int		edges;

	fd = open(filename, O_RDONLY);
	validate(fd, &ants_num, &verteces, &edges);
	close(fd);
	fd = open(filename, O_RDONLY);
	if (!(colony = (t_lemin *)malloc(sizeof(t_lemin))))
		error(-1);
	colony->adjacency = adjacency_matrix(edges);
	colony->verteces = verteces;
	colony->rooms = process_file(colony, fd);
	colony->ants_num = ants_num;
	colony->ants = populate_ants(colony->start, ants_num);
	close(fd);
	return (colony);
}
