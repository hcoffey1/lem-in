/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:37:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/21 16:41:00 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

void	depth_first_search(t_room *start)
{
	int i;

	i = 0;
	t_room *next;
	while (i < start->edges)
	{
		next = start->links[i++];
		while (next->type != 4)
			depth_first_search(next);
	}
}