/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/17 23:50:49 by smorty            #+#    #+#             */
/*   Updated: 2019/07/18 00:19:52 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"
#include "get_next_line.h"

/*
** status: 0 - none, 1 - start, 2 - end, 3 - comment
*/

static t_room	*read_line(char *line, int status)
{
	t_room	*new;
	int		spaces;
	char	**split;

	spaces = 0;
	new = (t_room *)malloc(sizeof(t_room));
	new->status = status;
	if (!(split = ft_strsplit(line, ' ')))
		exit(-1);
	while (*line)
		if (*line++ == ' ')
			++spaces;
	if (spaces != 3)
		exit(-1);
	new->name = ft_strdup(*split);
	free(*split);
	new->x = ft_atoi(*(split + 1));
	free(*(split + 1));
	new->y = ft_atoi(*(split + 2));
	free(*(split + 2));
	free(split);
}

static int		get_status(char *line)
{
	int status;

	status = 0;
	if (*line == '#')
	{
		if (*(line + 1) != '#')
			status = 3;
		else if (ft_strequ(line, "start"))
			status = 1;
		else if (ft_strequ(line, "start"))
			status = 2;
		else
			exit(-1);
	}
	return (status);
}

void			read_file(int fd)
{
	char	*line;
	int		ants;
	int		r;
	int		status;
	t_room	*room;

	r = get_next_line(fd, &line); //первая строка - количество муравьёв
	ants = ft_atoi(line);
	free(line);
	while (r = get_next_line(fd, &line))
	{
		status = 0;
		if (r < 0)
			exit(-1);
		if (*line == '#')
			status = get_status(line);
		else
			room = read_line(line, status);
		free(line);
	}
}