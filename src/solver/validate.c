/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 17:21:27 by smorty            #+#    #+#             */
/*   Updated: 2019/08/07 22:52:03 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	check_ants(t_input **map)
{
	char	*ants_line;
	int		ants;

	while ((*map)->type == COMMENT)
		*map = (*map)->next;
	if (!*map)
		error(ERR_ANTS);
	ants = ft_atoi((*map)->line);
	ants_line = ft_itoa(ants);
	if (ants < 0 || !ft_strequ((*map)->line, ants_line))
		error(ERR_ANTS);
	free(ants_line);
	*map = (*map)->next;
	return (ants);
}

static int	check_rooms(t_input **map)
{
	int rooms;
	int ends_num;

	rooms = 0;
	ends_num = 0;
	while (*map && (*map)->type != LINK)
	{
		if ((*map)->type == ROOM)
			++rooms;
		else if ((*map)->type == START || (*map)->type == END)
		{
			if (!(*map = (*map)->next) || (*map)->type != ROOM)
				error(ERR_ENDS);
			(*map)->type = (*map)->prev->type;
			(*map)->prev->type = COMMENT;
			ends_num += (*map)->type;
			++rooms;
		}
		else if ((*map)->type != COMMENT)
			error(ERR_ROOM);
		*map = (*map)->next;
	}
	if (rooms < 2 || ends_num != START + END)
		error(ERR_ENDS);
	return (rooms);
}

static void	check_links(t_input **map)
{
	int links;

	links = 0;
	while (*map)
	{
		if ((*map)->type == LINK)
			++links;
		else if ((*map)->type != COMMENT)
		{
			(*map)->prev->next = NULL;
			while ((*map)->next)
			{
				free((*map)->line);
				*map = (*map)->next;
				free((*map)->prev);
			}
			free((*map)->line);
			free(*map);
			return ;
		}
		*map = (*map)->next;
	}
	if (!links)
		error(ERR_PATH);
}

void		validate(t_input *map, int *ants, int *verteces)
{
	*ants = check_ants(&map);
	*verteces = check_rooms(&map);
	check_links(&map);
}
