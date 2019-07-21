/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smorty <smorty@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/21 16:37:02 by smorty            #+#    #+#             */
/*   Updated: 2019/07/21 22:08:54 by smorty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lemin.h"

int		depth_first_search(t_room *start)
{
	t_room	*next;
	int		i;

	if (start->type == 4)
	{
		printf("%s ", start->name);
		return (1);
	}
	i = 0;
	start->visited = 1;
	while (i < start->edges)
	{
		next = start->links[i++];
		if (!next->visited)
			if (depth_first_search(next))
				printf("%s ", start->name);
	}
	start->visited = 0;
	return (0);
}

int		 breadth_first_search(t_room *start, t_rooms_queue *q)
{
	t_rooms_queue *qptr;
	int		i;

	i = 0;
	if (start->type == 4)
	{
		while (start)
		{
			printf("| %s |", start->name);
			start->closed = 1;
			start = start->path;
			++i;
		}
		printf("\n");
		return (i);
	}
	start->visited = 1;
	qptr = q;
	while (qptr->next)
		qptr = qptr->next;
	while (start->links[i])
	{
		if (start->links[i]->visited == 0 && start->links[i]->closed == 0)
		{
			qptr->next = (t_rooms_queue *)malloc(sizeof(t_rooms_queue));
			qptr->next->val = start->links[i];
			qptr->next->val->path = start;
			qptr = qptr->next;
			qptr->next = NULL;
		}
		++i;
	}
	while (q && q->val->visited)
		q = q->next;
	if (!q)
		return (0);
	return (breadth_first_search(q->val, q));
}
