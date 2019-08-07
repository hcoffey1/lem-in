/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 23:05:05 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 22:52:03 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static char	*output_line(char *tail, char *p)
{
	char *line;
	char *tail0;

	if (!(line = (char *)malloc(sizeof(char) * (p - tail + 1))))
		error(errno);
	p = line;
	tail0 = tail;
	while (*tail && *tail != '\n')
		*p++ = *tail++;
	*p = 0;
	if (*tail)
		++tail;
	while (*tail)
		*tail0++ = *tail++;
	*tail0 = 0;
	return (line);
}

static char	*gnl(char **tail)
{
	char		buf[BUFF_SIZE + 1];
	char		*p;
	int			r;

	if ((r = read(0, buf, BUFF_SIZE)) < 0)
		error(errno);
	if (r)
	{
		buf[r] = 0;
		p = *tail;
		if (!(*tail = ft_strjoin(*tail, buf)))
			error(errno);
		free(p);
	}
	p = *tail;
	while (*p && *p != '\n')
		++p;
	if (*p)
		return (output_line(*tail, p));
	if (r)
		return (gnl(tail));
	return (NULL);
}

static int	identify_line(char *line)
{
	if (*line == '#')
	{
		if (ft_strequ(line, "##start"))
			return (START);
		if (ft_strequ(line, "##end"))
			return (END);
		return (COMMENT);
	}
	while (*line)
	{
		if (*line == ' ')
			return (ROOM);
		if (*line == '-')
			return (LINK);
		++line;
	}
	return (OTHER);
}

t_input		*read_input(t_input *prev)
{
	static char	*tail = NULL;
	t_input		*new;

	if (!tail)
	{
		if (!(tail = (char *)malloc(sizeof(char))))
			error(errno);
		*tail = 0;
	}
	if (!(new = (t_input *)malloc(sizeof(t_input))))
		error(errno);
	if (!(new->line = gnl(&tail)))
	{
		free(tail);
		free(new);
		return (NULL);
	}
	new->type = identify_line(new->line);
	new->prev = prev;
	new->next = read_input(new);
	return (new);
}

void		print_file(t_input *map)
{
	while (map->next)
	{
		ft_putstr(map->line);
		ft_putchar('\n');
		free(map->line);
		map = map->next;
		free(map->prev);
	}
	ft_putstr(map->line);
	ft_putstr("\n\n");
	free(map->line);
	free(map);
}
