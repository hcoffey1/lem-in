/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 23:05:05 by smorty            #+#    #+#             */
/*   Updated: 2019/08/06 18:15:49 by smorty           ###   ########.fr       */
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

t_mfile		*read_input(t_mfile *prev)
{
	static char	*tail = NULL;
	t_mfile		*new;

	if (!tail)
	{
		if (!(tail = (char *)malloc(sizeof(char))))
			error(errno);
		*tail = 0;
	}
	if (!(new = (t_mfile *)malloc(sizeof(t_mfile))))
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
