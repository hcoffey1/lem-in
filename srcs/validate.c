/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/05 17:21:27 by smorty            #+#    #+#             */
/*   Updated: 2019/08/05 23:45:52 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

static int	check_ants(t_mfile **map)
{
	int ants;

	while ((*map)->type == COMMENT)
		*map = (*map)->next;
	if (!*map)
		error(ERR_ANTS);
	ants = ft_atoi((*map)->line);
	if (ants < 0 || !ft_strequ((*map)->line, ft_itoa(ants)))
		error(ERR_ANTS);
	*map = (*map)->next;
	return (ants);
}

static int	check_rooms(t_mfile **map)
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
			*map = (*map)->next;
			if (!*map || (*map)->type != ROOM)
				error(ERR_STEN);
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
		error(ERR_STEN);
	return (rooms);
}

static void	check_links(t_mfile **map)
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
			free(map);
			return ;
		}
		*map = (*map)->next;
	}
	if (!links)
		error(ERR_LINK);
}

void		validate(t_mfile *map, int *ants, int *verteces)
{
	*ants = check_ants(&map);
	*verteces = check_rooms(&map);
	check_links(&map);
}
