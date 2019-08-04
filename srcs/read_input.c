/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 23:05:05 by smorty            #+#    #+#             */
/*   Updated: 2019/08/04 23:54:43 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static char	*output_line(char *tail, char *p)
{
	char *line;
	char *tail0;

	if (!(line = (char *)malloc(sizeof(char) * (p - tail))))
		error(errno);
	p = line;
	tail0 = tail;
	while (*tail && *tail != '\n')
		*p++ = *tail++;
	*p = 0;
	if (*tail)
		++tail;
	p = tail0;
	while (*tail)
		*tail0++ = *tail++;
	*tail0 = 0;
	return (line);
}

static char	*gnl(void)
{
	static char	*tail = NULL;
	char		buf[BUFF_SIZE + 1];
	char		*p;
	int			r;

	if (!tail)
	{
		if (!(tail = (char *)malloc(sizeof(char))))
			error(errno);
		*tail = 0;
	}
	if ((r = read(0, buf, BUFF_SIZE)) < 0)
		error(errno);
	if (!r && !*tail)
	{
		free(tail);
		return (NULL);
	}
	if (r)
	{
		buf[r] = 0;
		p = tail;
		if (!(tail = ft_strjoin(tail, buf)))
			error(errno);
		free(p);
	}
	p = tail;
	while (*p && *p != '\n')
		++p;
	if (*p)
		return (output_line(tail, p));
	return (gnl());
}

t_mfile		*read_input(t_mfile *prev)
{
	t_mfile	*new;
	char	*line;

	if (!(line = gnl()))
		return (NULL);
	if (!(new = (t_mfile *)malloc(sizeof(t_mfile))))
		error(errno);
	new->prev = prev;
	new->line = line;
	new->type = -1;
	ft_printf("%s\n", new->line);
	new->next = read_input(new);
	return (new);
}
